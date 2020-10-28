#pragma once

#include "internal_message.pb.h"
#include "message_dao.hpp"
#include <mysql++/mysql++.h>

// TODO: Why it work, no more linking error?
#define MYSQLPP_SSQLS_NO_STATICS
#include "message.h"

namespace detail {

model::Message makeMessage(const info::MessageInfo &msgInfo);
model::Message makeMessage(const msg::Chat &chat, const int64_t id);
info::MessageInfo fromMessage(const model::Message &msg);
} // namespace detail

template <class GenIdService>
class MessageDaoImpl
    : public MessageDao<MessageDaoImpl<GenIdService>, GenIdService> {

  friend class MessageDao<MessageDaoImpl<GenIdService>, GenIdService>;

private:
  template <class Config> void startImpl(const Config &config) {
    model::Message::table("messages");
    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  bool addImpl(const info::MessageInfo &msgInfo) {
    mysqlpp::Query query = conn.query();
    query.insert(detail::makeMessage(msgInfo));
    return query.exec();
  }

  uint64_t addImpl(const msg::Chat &chat) {
    mysqlpp::Query query = conn.query();
    const auto messageId = GenIdService::instance().nextId();
    query.insert(detail::makeMessage(chat, messageId));
    query.exec();
    return messageId;
  }

  mysqlpp::Connection conn;
};
