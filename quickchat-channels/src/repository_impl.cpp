//
// Created by giahuy on 10/18/20.
//

//#define MYSQLPP_SSQLS_NO_STATICS
#include "channel.h"
#include "member.h"

#include "channel.pb.h"
#include "channel_service.pb.h"
#include "gen_id_service_impl.h"
#include "logger.hpp"
#include "repository_impl.hpp"
#include <algorithm>
#include <cstdint>
#include <google/protobuf/empty.pb.h>
#include <google/protobuf/util/time_util.h>
#include <mysql++/datetime.h>
#include <mysql++/exceptions.h>
#include <mysql++/mysql++.h>
#include <mysql++/mystring.h>
#include <mysql++/query.h>
#include <mysql++/result.h>
#include <mysql++/row.h>
#include <mysql++/transaction.h>
#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string_view>

namespace detail {

std::string to_string(const mysqlpp::String &str);

msg::Channel parseChannel(const mysqlpp::Row &row);

msg::Channel::Member parseMember(const mysqlpp::Row &row);

std::pair<mysqlpp::StoreQueryResult::iterator, msg::Channel>
next(mysqlpp::StoreQueryResult::iterator begin,
     mysqlpp::StoreQueryResult::iterator end);

mysqlpp::sql_blob createToken(const uint64_t uid1, const uint64_t uid2);

} // namespace detail

msg::Channel
RepositoryImpl::getChannelImpl(const ::srv::GetChannelRequest *request) {
  constexpr auto queryString =
      "SELECT id, name, type, token, channels.create_at AS create_at, "
      "user_id, role, members.create_at AS join_at FROM channels JOIN "
      "members ON channels.id = members.channel_id WHERE id = %0";

  mysqlpp::Query query = conn.query(queryString);
  query.parse();
  auto queryResult = query.store(request->channel_id());
  auto [it, ch] = detail::next(queryResult.begin(), queryResult.end());
  // TODO: remove assert
  assert(it == queryResult.end());
  return ch;
}

msg::Channel RepositoryImpl::createPrivateChannel(
    const ::srv::CreateChannelRequest *request) {

  // TODO: is this transaction level correct
  mysqlpp::Transaction tran(conn, mysqlpp::Transaction::read_committed,
                            mysqlpp::Transaction::this_transaction);

  mysqlpp::Query query = conn.query();
  const uint64_t channelId = GenIdServiceImpl::instance().nextId();
  if (request->channel().members().size() != 2) {
    throw mysqlpp::BadQuery("members.size() != 2");
  }

  const mysqlpp::sql_blob_null token =
      detail::createToken(request->channel().members(0).user_id(),
                          request->channel().members(1).user_id());
  model::Channel channel = model::Channel(channelId, mysqlpp::null, "private",
                                          token, mysqlpp::NOW(), mysqlpp::null);
  query.insert(channel);
  query.exec();

  mysqlpp::DateTime joinAt = mysqlpp::NOW();
  for (const auto &member : request->channel().members()) {
    query.insert(model::Member(channelId, member.user_id(), "user", joinAt,
                               mysqlpp::null));
    query.exec();
  }
  tran.commit();
  // TODO: fix it
  srv::GetChannelRequest req;
  req.set_channel_id(channelId);

  msg::Channel result = getChannel(&req);
  return result;
}

msg::Channel
RepositoryImpl::createGroupChannel(const ::srv::CreateChannelRequest *request) {
  // TODO: is this transaction level correct
  mysqlpp::Transaction tran(conn, mysqlpp::Transaction::read_committed,
                            mysqlpp::Transaction::this_transaction);

  mysqlpp::Query query = conn.query();
  const uint64_t channelId = GenIdServiceImpl::instance().nextId();
  if (request->channel().members().size() == 1) {
    throw mysqlpp::BadQuery("members.size() == 1");
  }

  model::Channel channel =
      model::Channel(channelId, request->channel().name(), "group",
                     mysqlpp::null, mysqlpp::NOW(), mysqlpp::null);
  query.insert(channel);
  query.exec();

  mysqlpp::DateTime joinAt = mysqlpp::NOW();
  bool first = true;
  for (const auto &member : request->channel().members()) {
    query.insert(model::Member(channelId, member.user_id(),
                               first ? "creator" : "user", joinAt,
                               mysqlpp::null));
    query.exec();
    first = false;
  }
  tran.commit();
  // TODO: fix it
  srv::GetChannelRequest req;
  req.set_channel_id(channelId);

  msg::Channel result = getChannel(&req);
  return result;
}

msg::Channel
RepositoryImpl::createChannelImpl(const ::srv::CreateChannelRequest *request) {

  if (request->channel().type() == "private") {
    return createPrivateChannel(request);
  } else if (request->channel().type() == "group") {
    return createGroupChannel(request);
  } else {
    throw mysqlpp::BadQuery("Invalid channel type");
  }
}

msg::Channel
RepositoryImpl::updateChannelImpl(const ::srv::UpdateChannelRequest *request) {
  msg::Channel channel;
  throw std::runtime_error("Unimplement");
  return channel;
}

google::protobuf::Empty
RepositoryImpl::deleteChannelImpl(const ::srv::DeleteChannelRequest *request) {
  throw std::runtime_error("Unimplement");
  return {};
}

::srv::ListUserChannelsReponse RepositoryImpl::listUserChannelsImpl(
    const ::srv::ListUserChannelsRequest *request) {
  constexpr auto queryString =
      "SELECT channels.id, name, type, token, channels.create_at AS "
      "create_at, token, user_id, role, members.create_at AS join_at  FROM "
      "(SELECT id FROM channels WHERE id IN (SELECT channel_id FROM members "
      "WHERE user_id = %0)) user_channel JOIN channels ON user_channel.id = "
      "channels.id JOIN members on members.channel_id = user_channel.id";
  mysqlpp::Query query = conn.query(queryString);
  query.parse();
  auto queryResult = query.store(request->user_id());

  auto first = std::begin(queryResult);

  srv::ListUserChannelsReponse res;

  while (first != std::end(queryResult)) {
    auto [next, channel] = detail::next(first, std::end(queryResult));
    res.mutable_channels()->Add(std::move(channel));
    first = next;
  }

  return res;
}

::srv::CheckIsMemberChannelReponse RepositoryImpl::checkIsMemberChannelImpl(
    const ::srv::CheckIsMemberChannelRequest *request) {
  constexpr auto queryString =
      "SELECT EXISTS(SELECT 1 FROM members WHERE channel_id=%0 AND "
      "user_id=%1 LIMIT 1) AS value";
  mysqlpp::Query query = conn.query(queryString);
  query.parse();
  auto queryResult = query.store(request->channel_id(), request->user_id());
  const bool isIn = queryResult[0]["value"];
  srv::CheckIsMemberChannelReponse res;
  res.set_channel_id(request->channel_id());
  res.set_user_id(request->user_id());
  res.set_is_in(isIn);
  return res;
}

::srv::CheckIsMemberAndGetChannelReponse
RepositoryImpl::checkIsMemberAndGetChannelImpl(
    const ::srv::CheckIsMemberAndGetChannelRequest *request) {

  // TODO: refactor this
  srv::CheckIsMemberChannelRequest isMemberReq;
  isMemberReq.set_channel_id(request->channel_id());
  isMemberReq.set_user_id(request->user_id());

  srv::CheckIsMemberAndGetChannelReponse result;
  result.set_is_in(this->checkIsMemberChannel(&isMemberReq).is_in());

  if (!result.is_in()) {
    return result;
  }

  srv::GetChannelRequest getChannelReq;
  getChannelReq.set_channel_id(request->channel_id());

  *result.mutable_channel() = this->getChannel(&getChannelReq);
  return result;
}

msg::Channel RepositoryImpl::addMembersChannelImpl(
    const ::srv::AddMembersChannelRequest *request) {
  mysqlpp::DateTime joinAt = mysqlpp::NOW();
  mysqlpp::Transaction tran(conn, mysqlpp::Transaction::read_committed,
                            mysqlpp::Transaction::this_transaction);

  mysqlpp::Query query = conn.query();
  const uint64_t channelId = request->channel_id();
  for (const msg::Channel_Member &mem : request->members()) {
    query.insert(
        model::Member(channelId, mem.user_id(), "user", joinAt, mysqlpp::null));
    query.exec();
  }

  // TODO: fix it
  srv::GetChannelRequest req;
  req.set_channel_id(channelId);

  msg::Channel channel = getChannel(&req);
  if (channel.type() == "group") {
    tran.commit();
    return channel;
  } else {
    tran.rollback();
    return getChannel(&req);
  }
}

msg::Channel RepositoryImpl::removeMemberChannelImpl(
    const ::srv::RemoveMemberChannelRequest *request) {
  constexpr auto queryString =
      "DELETE FROM members WHERE channel_id = %0 AND user_id = %1";
  mysqlpp::Query query = conn.query(queryString);
  query.parse();

  const uint64_t channelId = request->channel_id();
  const uint64_t userId = request->user_id();
  query.execute(channelId, userId);

  // TODO: fix it
  srv::GetChannelRequest req;
  req.set_channel_id(channelId);

  msg::Channel channel = getChannel(&req);
  return channel;
}

msg::Channel RepositoryImpl::getPrivateChannelImpl(
    const ::srv::GetPrivateChannelRequest *request) {

  const mysqlpp::sql_blob token =
      detail::createToken(request->uid1(), request->uid2());

  constexpr auto queryString =
      "SELECT id, name, type, token, channels.create_at AS create_at, "
      "user_id, role, members.create_at AS join_at FROM channels JOIN "
      "members ON channels.id = members.channel_id WHERE token = %0q";

  mysqlpp::Query query = conn.query(queryString);
  query.parse();

  // logger::info(query.str(token));
  auto queryResult = query.store(token);
  // logger::info(queryResult.size());
  if (queryResult.empty()) {
    throw mysqlpp::BadQuery("private channel does not exists");
  }
  auto [it, ch] = detail::next(queryResult.begin(), queryResult.end());
  // TODO: remove assert
  // assert(it == queryResult.end());
  return ch;
}

mysqlpp::sql_blob detail::createToken(const uint64_t uid1,
                                      const uint64_t uid2) {
  const auto [small, large] = std::minmax(uid1, uid2);
  if (small == large) {
    throw std::runtime_error("duplicate user id");
  }
  return mysqlpp::sql_blob(fmt::format("{}{}", small, large));
}

std::pair<mysqlpp::StoreQueryResult::iterator, msg::Channel>
detail::next(mysqlpp::StoreQueryResult::iterator begin,
             mysqlpp::StoreQueryResult::iterator end) {
  if (begin == end) {
    return {end, {}};
  }
  const uint64_t currentId = (*begin)["id"];
  msg::Channel channel = parseChannel(*begin);

  channel.mutable_members()->Add(parseMember(*begin));
  ++begin;
  while (begin != end) {
    const uint64_t nextId = (*begin)["id"];
    if (currentId != nextId) {
      break;
    }
    channel.mutable_members()->Add(parseMember(*begin));
    ++begin;
  }
  return {begin, channel};
}

msg::Channel::Member detail::parseMember(const mysqlpp::Row &row) {
  msg::Channel::Member result;
  result.set_user_id(row["user_id"]);
  result.set_role(row["role"]);

  mysqlpp::DateTime joinAt = row["join_at"];
  *result.mutable_join_at() =
      google::protobuf::util::TimeUtil::SecondsToTimestamp(joinAt);
  return result;
}

msg::Channel detail::parseChannel(const mysqlpp::Row &row) {
  msg::Channel result;
  result.set_id(row["id"]);
  result.set_type(row["type"]);

  mysqlpp::sql_varchar_null token = row["token"];
  if (token.is_null) {
    result.set_type("group");
  } else {
    result.set_type("private");
  }

  mysqlpp::sql_varchar_null name = row["name"];

  if (!name.is_null) {
    result.set_name(name.data);
  }

  mysqlpp::DateTime createAt = row["create_at"];

  *result.mutable_create_at() =
      google::protobuf::util::TimeUtil::SecondsToTimestamp(createAt);
  return result;
}

std::string detail::to_string(const mysqlpp::String &str) {
  return {str.data(), str.size()};
}
