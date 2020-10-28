#pragma once

#include "internal_message.pb.h"
#include "logger.hpp"
#include <mysql++/datetime.h>
#include <mysql++/query.h>

//#define MYSQLPP_SSQLS_NO_STATICS
#include "message.h"
#include "seen.h"

#include "message.pb.h"
#include "message_repository.hpp"
#include "message_service.pb.h"
#include <algorithm>
#include <google/protobuf/util/time_util.h>
#include <mysql++/connection.h>
#include <mysql++/row.h>

namespace detail {

msg::Message toReponseMessage(const mysqlpp::Row &row) {
  const model::Message &message = row;
  msg::Message result;
  result.set_channel_id(message.channelId);
  result.set_user_id(message.userId);
  result.set_message_id(message.messageId);
  result.mutable_create_at()->set_seconds(message.createAt);

  msg::ChatContent chatContent;
  chatContent.ParseFromArray(message.content.data(), message.content.size());
  *result.mutable_content() = std::move(chatContent);
  return result;
}

model::Message toMessage(const msg::Chat &chat) {
  return model::Message(chat.channel_id(), chat.message_id(), chat.user_id(),
                        mysqlpp::sql_blob(chat.content().SerializeAsString()),
                        mysqlpp::NOW(), mysqlpp::null);
}

model::Seen toSeen(const msg::Seen &seen) {
  return model::Seen(seen.channel_id(), seen.message_id(), seen.user_id(),
                     mysqlpp::NOW());
}

} // namespace detail

template <class GenIdService>
class MessageRepositoryImpl
    : public MessageRepository<MessageRepositoryImpl<GenIdService>> {
public:
  friend class MessageRepository<MessageRepositoryImpl<GenIdService>>;

private:
  template <class Config> void startImpl(const Config &config) {
    model::Message::table("messages");
    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  // TODO: refactor
  srv::GetMessageReponse getMessageImpl(const srv::GetMessageRequest *req) {
    mysqlpp::Query query = conn.query();
    query << "SELECT channel_id, message_id, user_id, content, create_at FROM "
             "messages WHERE channel_id = "
          << req->channel_id() << " AND message_id = "
          << req->message_id(); // << " ORDER BY create_at DESC";
    auto queryResult = query.store();

    srv::GetMessageReponse res;
    if (queryResult.empty()) {
      return res;
    }
    *res.mutable_message() = detail::toReponseMessage(queryResult[0]);
    return res;
  }

  // TODO: refactor
  srv::GetLastMessageReponse
  getLastMessageImpl(const srv::GetLastMessageRequest *req) {
    // constexpr auto SEEN_QUERY =
    //    "SELECT channel_id, user_id, message_id, create_at FROM seens WHERE "
    //    "channel_id = %0 AND message_id = %1";
    //

    mysqlpp::Query query = conn.query();
    query << "SELECT channel_id, message_id, user_id, content, create_at FROM "
             "messages WHERE channel_id="
          << req->channel_id()
          << " AND message_id=(SELECT MAX(message_id) "
             "FROM messages WHERE channel_id="
          << req->channel_id() << ")";
    //logger::info(query.str());
    auto messageQueryResult = query.store();
    //logger::info(messageQueryResult.size());

    srv::GetLastMessageReponse res;
    if (messageQueryResult.empty()) {
      return res;
    }

    *res.mutable_message() = detail::toReponseMessage(messageQueryResult[0]);

    // query = conn.query(SEEN_QUERY);
    // query.parse();
    //
    // auto seenQueryResult =
    //    query.store(res.message().channel_id(), res.message().message_id());
    //
    // std::for_each(std::begin(seenQueryResult), std::end(seenQueryResult),
    //              [&](const model::Seen &seen) {
    //                msg::Message::Seen cur;
    //                cur.set_user_id(seen.userId);
    //                //cur.
    //                res.mutable_message()->mutable_seens()->Add();
    //              });
    //
    return res;
  }

  srv::GetMessagesReponse getMessagesImpl(const srv::GetMessagesRequest *req) {
    // TODO: real implement
    auto query = conn.query();
    query << "SELECT channel_id, message_id, user_id, content, create_at FROM "
             "messages WHERE channel_id="
          << req->channel_id(); // << " ORDER BY create_at DESC";

    auto queryResult = query.store();

    srv::GetMessagesReponse res;
    std::for_each(std::begin(queryResult), std::end(queryResult),
                  [&](const auto &row) {
                    res.mutable_messages()->Add(detail::toReponseMessage(row));
                  });
    return res;
  }

  bool saveImpl(msg::Chat &chat) {
    mysqlpp::Query query = conn.query();
    const uint64_t messageId = GenIdService::instance().nextId();
    chat.set_message_id(messageId);
    query.insert(detail::toMessage(chat));
    return query.exec();
  }

  bool saveImpl(const msg::Seen &seen) {
    mysqlpp::Query query = conn.query();
    query.insert(detail::toSeen(seen));
    return query.exec();
  }

private:
  mysqlpp::Connection conn;
};
