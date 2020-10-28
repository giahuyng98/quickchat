#pragma once

#include "internal_message.pb.h"
#include "message.pb.h"
#include "message_service.pb.h"

template <class T> class MessageRepository {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  inline srv::GetMessageReponse getMessage(const srv::GetMessageRequest *req) {
    return static_cast<T &>(*this).getMessageImpl(req);
  }

  inline srv::GetLastMessageReponse
  getLastMessage(const srv::GetLastMessageRequest *req) {
    return static_cast<T &>(*this).getLastMessageImpl(req);
  }

  inline srv::GetMessagesReponse
  getMessages(const srv::GetMessagesRequest *req) {
    return static_cast<T &>(*this).getMessagesImpl(req);
  }

  inline auto save(msg::Chat &chat) {
    return static_cast<T &>(*this).saveImpl(chat);
  }

  inline auto save(const msg::Seen &seen) {
    return static_cast<T &>(*this).saveImpl(seen);
  }
};
