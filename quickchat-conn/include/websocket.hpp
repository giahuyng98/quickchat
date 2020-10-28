#pragma once
#include "singleton.hpp"
#include <string_view>

template <class WS> class WebSocket : public Singleton<WS> {
public:
  friend class Singleton<WS>;

  WebSocket() = default;

  template <class Config> void start(const Config &config) {
    static_cast<WS &>(*this).startImpl(config);
  }

  void handleChatMessage(const std::string_view message) {
    static_cast<WS &>(*this).handleChatMessageImpl(message);
  }

  void handleSeenMessage(const std::string_view message) {
    static_cast<WS &>(*this).handleSeenMessageImpl(message);
  }

  void handleTypingMessage(const std::string_view message) {
    static_cast<WS &>(*this).handleTypingMessageImpl(message);
  }

  void handleStatusMessage(const std::string_view message) {
    static_cast<WS &>(*this).handleStatusMessageImpl(message);
  }

  void handleFriendNotification(const std::string_view message) {
    static_cast<WS &>(*this).handleFriendNotificationImpl(message);
  }

  void handleGroupNotification(const std::string_view message) {
    static_cast<WS &>(*this).handleGroupNotificationImpl(message);
  }
};
