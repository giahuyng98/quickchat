#pragma once

#include <string_view>

//#define VIRTUAL_FUNC(...)

template <class T> class MessageProducer {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  inline auto produceChatMessage(const std::string_view message) {
    return static_cast<T *>(this)->produceChatMessageImpl(message);
  }

  inline auto produceSeenMessage(const std::string_view message) {
    return static_cast<T *>(this)->produceSeenMessageImpl(message);
  }

  inline auto produce(const std::string_view topic,
                      const std::string_view message) {
    return static_cast<T *>(this)->produceImpl(topic, message);
  }

  inline auto publishStatus(const std::string_view message) {
    return static_cast<T *>(this)->publishStatusImpl(message);
  }

  inline auto publishTyping(const std::string_view message) {
    return static_cast<T *>(this)->publishTypingImpl(message);
  }

  inline auto publish(const std::string_view channel,
                      const std::string_view message) {
    return static_cast<T *>(this)->publishImpl(channel, message);
  }
};
