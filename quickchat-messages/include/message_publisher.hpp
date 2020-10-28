#pragma once

#include "internal_message.pb.h"
#include <string_view>
template <class T> class MessagePublisher {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  inline auto publishChatMessage(const msg::Chat &message) {
    static_cast<T &>(*this).publishChatMessageImpl(message);
  }

  inline auto publishSeenMessage(const msg::Seen message) {
    static_cast<T &>(*this).publishSeenMessageImpl(message);
  }

  inline auto publishSeenMessage(const std::string_view message) {
    static_cast<T &>(*this).publishSeenMessageImpl(message);
  }
};
