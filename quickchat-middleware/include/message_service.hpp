#pragma once

#include <cstdint>

template <class T> class MessageService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }
  inline auto getLastMessage(const uint64_t channelId) {
    return static_cast<T &>(*this).getLastMessageImpl(channelId);
  }
  inline auto getMessages(const uint64_t channelId,
                          const uint64_t messageId = 0,
                          const int32_t count = 0) {
    return static_cast<T &>(*this).getMessagesImpl(channelId);
  }
};
