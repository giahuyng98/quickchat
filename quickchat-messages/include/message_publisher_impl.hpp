#pragma once

#include "internal_message.pb.h"
#include "message_publisher.hpp"
#include <functional>
#include <memory>
#include <sw/redis++/redis.h>

class MessagePublisherImpl : public MessagePublisher<MessagePublisherImpl> {
public:
  friend class MessagePublisher<MessagePublisherImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    client = std::make_unique<sw::redis::Redis>(
        config["redis"]["uri"].template get<std::string>());

    chatChannel =
        config["redis"]["publisher"]["chat"].template get<std::string>();

    seenChannel =
        config["redis"]["publisher"]["seen"].template get<std::string>();
  }

  void publishChatMessageImpl(const msg::Chat &message) {
    client->publish(chatChannel, message.SerializeAsString());
  }

  void publishSeenMessageImpl(const msg::Seen &message) {
    client->publish(seenChannel, message.SerializeAsString());
  }

  void publishSeenMessageImpl(const std::string_view message) {
    client->publish(seenChannel, message);
  }

private:
  std::string chatChannel;
  std::string seenChannel;
  std::unique_ptr<sw::redis::Redis> client;
};
