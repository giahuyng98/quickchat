#pragma once

#include "internal_message.pb.h"
#include "logger.hpp"
#include "message.pb.h"
#include "message_consumer.hpp"
#include <cppkafka/configuration.h>
#include <cppkafka/consumer.h>
#include <cppkafka/cppkafka.h>
#include <cppkafka/exceptions.h>
#include <cppkafka/message.h>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string_view>

#include "message_repository_impl.hpp"
#include "message_service.pb.h"

namespace detail {

// TODO: wrong interface
template <class Config>
cppkafka::Configuration readConfig(const Config &config) {
  std::vector<cppkafka::ConfigurationOption> kafkaConfig;

  for (const auto &[k, v] : config["kafka"]["config"].items()) {
    kafkaConfig.emplace_back(
        cppkafka::ConfigurationOption(k, v.template get<std::string>()));
  }
  return kafkaConfig;
}

// Parse chat message from binary
std::optional<msg::Chat> parseChatMessage(const std::string_view bin) {
  msg::Chat chat;
  if (chat.ParseFromArray(bin.data(), bin.size())) {
    return chat;
  }
  return std::nullopt;
}

// Parse seen message from binary
std::optional<msg::Seen> parseSeenMessage(const std::string_view bin) {
  msg::Seen seen;
  if (seen.ParseFromArray(bin.data(), bin.size())) {
    return seen;
  }
  return std::nullopt;
}

} // namespace detail

template <class Publisher, class MessageRepository>
class MessageConsumerImpl
    : public MessageConsumer<MessageConsumerImpl<Publisher, MessageRepository>,
                             Publisher> {

  friend class MessageConsumer<
      MessageConsumerImpl<Publisher, MessageRepository>, Publisher>;

private:
  std::string chatTopic;
  std::string seenTopic;

  void handleChatMessage(const std::string_view msg) {
    std::optional<msg::Chat> chat = detail::parseChatMessage(msg);

    if (!chat) {
      logger::error("can't parse chat message");
      return;
    }

    try {
      if (repo.save(*chat)) {
        // ok message is saved, now publish it
        publisher.publishChatMessage(*chat);
      }

    } catch (std::exception &ex) {
      logger::error("can't save chat message");
    }

    // saving or not, just commit
    consumer->commit();
  }

  void handleSeenMessage(const std::string_view msg) {
    std::optional<msg::Seen> seen = detail::parseSeenMessage(msg);

    if (!seen) {
      logger::error("can't parse seen message");
      return;
    }
    try {
      if (repo.save(*seen)) {
        // ok message is saved, now publish it
        publisher.publishSeenMessage(msg);
      }

    } catch (std::exception &ex) {
      logger::error("can't save chat message");
    }

    // saving or not, just commit
    consumer->commit();
  }

  void loopImpl() {
    while (true) {
      try {
        cppkafka::Message msg = consumer->poll();
        if (!msg) {
          continue;
        }

        if (msg.get_error()) {
          logger::error("Kafka error message: {}", msg.get_error().to_string());
        } else {
          std::string_view binaryMessage{(char *)msg.get_payload().get_data(),
                                         msg.get_payload().get_size()};

          const std::string topic = msg.get_topic();

          logger::info("Kafka [{}] : {}", topic, binaryMessage);

          if (topic == chatTopic) {
            handleChatMessage(binaryMessage);
          } else if (topic == seenTopic) {
            handleSeenMessage(binaryMessage);
          } else {
            logger::error("kafka receive unknow topic: {}", topic);
          }
        }
      } catch (cppkafka::Exception &ex) {
        // TODO: ???
        logger::error(ex.what());
      }
    }
    logger::error("kafka stopped");
  }

  template <class Config> void startImpl(const Config &config) {

    repo.start(config);

    publisher.start(config);

    chatTopic = config["kafka"]["consumer"]["topics"]["chat"]
                    .template get<std::string>();

    seenTopic = config["kafka"]["consumer"]["topics"]["seen"]
                    .template get<std::string>();

    consumer = std::make_unique<cppkafka::Consumer>(detail::readConfig(config));
    consumer->subscribe({chatTopic, seenTopic});
  }

private:
  MessageRepository repo;
  // TODO: do we realy need pointer here?
  std::unique_ptr<cppkafka::Consumer> consumer;
  Publisher publisher;
};
