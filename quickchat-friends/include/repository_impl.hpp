#pragma once

#include "friend.hpp"
#include "friend.pb.h"
#include "friend_service.pb.h"
#include "friends_request.hpp"
#include "logger.hpp"
#include "repository.hpp"
#include <algorithm>
#include <exception>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/util/time_util.h>
#include <mysql++/exceptions.h>
#include <mysql++/mysql++.h>
#include <mysql++/query.h>
#include <mysql++/result.h>
#include <mysql++/row.h>
#include <mysql++/transaction.h>
#include <optional>

namespace detail {
msg::Friend toFriend(const uint64_t userId, const mysqlpp::Row &row) {
  msg::Friend result;

  const uint64_t uid1 = row["uid1"];
  const uint64_t uid2 = row["uid2"];
  result.set_user_id(uid1 == userId ? uid2 : uid1);
  result.set_status(row["status"]);
  result.mutable_create_at();

  mysqlpp::DateTime createAt = row["create_at"];

  *result.mutable_create_at() =
      google::protobuf::util::TimeUtil::SecondsToTimestamp(createAt);
  return result;
}

mysqlpp::sql_blob createToken(const uint64_t uid1, const uint64_t uid2) {
  const auto [small, large] = std::minmax(uid1, uid2);
  if (small == large) {
    throw std::runtime_error("duplicate user id");
  }
  return mysqlpp::sql_blob(fmt::format("{}{}", small, large));
}

std::pair<uint64_t, uint64_t> swapIfNeed(const uint64_t uid1,
                                         const uint64_t uid2) {
  const auto [small, large] = std::minmax(uid1, uid2);
  if (small == large) {
    throw std::runtime_error("duplicate user id");
  }
  return std::make_pair(small, large);
}

} // namespace detail

class RepositoryImpl : public Repository<RepositoryImpl> {
  friend class Repository<RepositoryImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    model::FriendRequest::table("friends_request");
    model::Friend::table("friends");
    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  auto getListFriends(const uint64_t userId) {
    mysqlpp::Query query = conn.query();
    query << "SELECT uid1, uid2, status, create_at FROM "
             "friends WHERE uid1 = "
          << userId << " OR uid2 = " << userId;

    const mysqlpp::StoreQueryResult queryResult = query.store();

    google::protobuf::RepeatedPtrField<msg::Friend> result;
    std::for_each(std::begin(queryResult), std::end(queryResult),
                  [&](const mysqlpp::Row &row) {
                    result.Add(detail::toFriend(userId, row));
                  });
    return result;
  }

  // TODO: combine 2 function sent & receive
  auto getSentList(const uint64_t userId) {
    mysqlpp::Query query = conn.query();
    query << "SELECT uid1, uid2, status, create_at FROM "
             "friends_request WHERE status = 'waiting' AND sender = "
          << userId;

    const mysqlpp::StoreQueryResult queryResult = query.store();

    google::protobuf::RepeatedPtrField<msg::Friend> result;
    std::for_each(std::begin(queryResult), std::end(queryResult),
                  [&](const mysqlpp::Row &row) {
                    result.Add(detail::toFriend(userId, row));
                  });
    return result;
  }

  auto getReceiveList(const uint64_t userId) {
    mysqlpp::Query query = conn.query();
    query << "SELECT uid1, uid2, status, create_at FROM friends_request WHERE "
             "status = 'waiting' AND sender != "
          << userId << " AND (uid1 = " << userId << " OR uid2 = " << userId
          << ")";

    const mysqlpp::StoreQueryResult queryResult = query.store();

    google::protobuf::RepeatedPtrField<msg::Friend> result;
    std::for_each(std::begin(queryResult), std::end(queryResult),
                  [&](const mysqlpp::Row &row) {
                    result.Add(detail::toFriend(userId, row));
                  });
    return result;
  }

  srv::ListUserFriendsReponse
  listUserFriendsImpl(const ::srv::ListUserFriendsRequest *req) {
    srv::ListUserFriendsReponse res;
    *res.mutable_friends() = getListFriends(req->user_id());
    *res.mutable_sent_requests() = getSentList(req->user_id());
    *res.mutable_received_request() = getReceiveList(req->user_id());
    return res;
  }

  bool createNewRequest(const uint64_t uid1, const uint64_t uid2,
                        const uint64_t sender) {
    model::FriendRequest friendRequest(uid1, uid2, sender, "waiting",
                                       mysqlpp::NOW());
    mysqlpp::Query query = conn.query();
    query.insert(friendRequest);
    query.execute();
    return query.affected_rows() != 0;
  }

  bool createNewFriend(const uint64_t uid1, const uint64_t uid2) {
    model::Friend f(uid1, uid2, "friend", mysqlpp::NOW());
    mysqlpp::Query query = conn.query();
    query.insert(f);
    query.execute();
    return query.affected_rows() != 0;
  }

  bool updateStatus(const uint64_t uid1, const uint64_t uid2,
                    const std::string &status) {
    constexpr auto queryString =
        "UPDATE friends_request SET status = %0q WHERE uid1 = %1 AND uid2 = %2";
    mysqlpp::Query query = conn.query(queryString);
    query.parse();
    query.execute(status, uid1, uid2);
    return query.affected_rows() != 0;
  }

  srv::BaseReponse createFriendImpl(const ::srv::BaseRequest *req) {
    const uint64_t sender = req->from();
    const uint64_t receiver = req->to();
    const auto [uid1, uid2] = detail::swapIfNeed(sender, receiver);

    srv::BaseReponse result;
    result.set_from(req->from());
    result.set_to(req->to());

    mysqlpp::Transaction tran(conn, mysqlpp::Transaction::read_committed,
                              mysqlpp::Transaction::this_transaction);

    const auto fr = getFriendRequest(uid1, uid2);
    if (!fr) {
      const bool res = createNewRequest(uid1, uid2, sender);
      if (res) {
        tran.commit();
        result.set_message("create new request successfully");
        return result;
      }
      result.set_message("fail to create new request");
      return result;
    }

    if (fr->sender == sender) {
      result.set_message(
          fmt::format("request is already sent: {}", fr->status));
      return result;
    }

    if (fr->status == "blocked") {
      result.set_message("blocked by user");
      return result;
    }

    if (fr->status == "waiting") {
      if (!updateStatus(uid1, uid2, "accepted")) {
        result.set_message("fail update status");
        return result;
      }

      if (!createNewFriend(uid1, uid2)) {
        result.set_message("fail create new friend");
        return result;
      }

      tran.commit();
      result.set_message("ok");
      return result;
    }

    if (fr->status == "accepted") {
      result.set_message("already friend");
      return result;
    }
    return result;
  }

  bool removeRequest(const uint64_t uid1, const uint64_t uid2) {
    constexpr auto queryString =
        "DELETE FROM friends_request WHERE uid1 = %0 AND uid2 = %1";
    mysqlpp::Query query = conn.query(queryString);
    query.parse();
    query.execute(uid1, uid2);
    return query.affected_rows();
  }

  bool removeFriend(const uint64_t uid1, const uint64_t uid2) {
    constexpr auto queryString =
        "DELETE FROM friends WHERE uid1 = %0 AND uid2 = %1";
    mysqlpp::Query query = conn.query(queryString);
    query.parse();
    query.execute(uid1, uid2);
    return query.affected_rows();
  }

  std::optional<model::FriendRequest> getFriendRequest(const uint64_t uid1,
                                                       const uint64_t uid2) {
    constexpr auto queryString =
        "SELECT * FROM friends_request WHERE uid1 = %0 AND uid2 = %1";
    mysqlpp::Query query = conn.query(queryString);
    query.parse();
    mysqlpp::StoreQueryResult queryResult = query.store(uid1, uid2);
    if (queryResult.empty())
      return std::nullopt;
    return queryResult[0];
  }

  std::optional<model::Friend> getFriend(const uint64_t uid1,
                                         const uint64_t uid2) {
    constexpr auto queryString =
        "SELECT * FROM friends WHERE uid1 = %0 AND uid2 = %1";
    mysqlpp::Query query = conn.query(queryString);
    query.parse();
    mysqlpp::StoreQueryResult queryResult = query.store(uid1, uid2);
    if (queryResult.empty())
      return std::nullopt;
    return queryResult[0];
  }

  srv::BaseReponse deleteFriendImpl(const ::srv::BaseRequest *req) {
    srv::BaseReponse result;
    result.set_from(req->from());
    result.set_to(req->to());

    const uint64_t sender = req->from();
    const uint64_t receiver = req->to();
    const auto [uid1, uid2] = detail::swapIfNeed(sender, receiver);

    mysqlpp::Transaction tran(conn, mysqlpp::Transaction::read_committed,
                              mysqlpp::Transaction::this_transaction);
    {
      const auto friendModel = getFriend(uid1, uid2);
      const auto friendRequestModel = getFriendRequest(uid1, uid2);

      if (!friendModel && !friendRequestModel) {
        result.set_message("invalid request");
        return result;
      }

      if (friendModel && friendRequestModel) {
        if (removeFriend(uid1, uid2) && updateStatus(uid1, uid2, "canceled")) {
          tran.commit();
          result.set_message("ok");
          return result;
        }
        result.set_message("error, can't remove");
        return result;
      }

      if (friendRequestModel) {
        std::string msg = friendRequestModel->sender ? "canceled" : "declined";
        bool res = updateStatus(uid1, uid2, msg);
        tran.commit();
        result.set_message(res ? "ok: " + msg : "fail");
        return result;
      }
    }
    result.set_message("fall back");
    return result;
  }

  srv::BaseReponse
  changeFriendStatusImpl(const ::srv::ChangeFriendStatusRequest *req) {
    throw mysqlpp::BadQuery("not implement");
  }

private:
  mysqlpp::Connection conn;
};
