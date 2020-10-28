#pragma once
#include "logger.hpp"
#include "message_producer.hpp"
#include <chrono>
#include <cppkafka/configuration.h>
#include <cppkafka/cppkafka.h>
#include <cppkafka/message.h>
#include <cppkafka/message_builder.h>
#include <cppkafka/producer.h>
#include <exception>
#include <memory>
#include <string>
#include <sw/redis++/redis.h>

class MessageProducerKafka : public MessageProducer<MessageProducerKafka> {

  std::string chatTopic;
  std::string seenTopic;
  std::string statusChannel;
  std::string typingChannel;

public:
  friend class MessageProducer<MessageProducerKafka>;
  MessageProducerKafka() = default;

private:
  template <class Config> void startImpl(const Config &config) {

    cppkafka::Configuration kafkaConfig = {
        {"bootstrap.servers", config["kafka"]["config"]["bootstrap.servers"]
                                  .template get<std::string>()}};

    producer = std::make_unique<cppkafka::Producer>(kafkaConfig);

    redisClient = std::make_unique<sw::redis::Redis>(
        config["redis"]["uri"].template get<std::string>());

    chatTopic =
        config["kafka"]["config"]["topic"]["chat"].template get<std::string>();

    seenTopic =
        config["kafka"]["config"]["topic"]["seen"].template get<std::string>();

    statusChannel =
        config["redis"]["publisher"]["status"].template get<std::string>();

    typingChannel =
        config["redis"]["publisher"]["typing"].template get<std::string>();
  }

  void publishStatusImpl(const std::string_view message) {
    redisClient->publish(statusChannel, message);
  }

  void publishTypingImpl(const std::string_view message) {
    redisClient->publish(typingChannel, message);
  }

  void publishImpl(const std::string_view channel,
                   const std::string_view message) {
    redisClient->publish(channel, message);
  }

  bool produceChatMessageImpl(const std::string_view message) {
    return produceImpl(chatTopic, message);
  }

  bool produceSeenMessageImpl(const std::string_view message) {
    return produceImpl(seenTopic, message);
  }

  bool produceImpl(const std::string_view topic,
                   const std::string_view message) {
    try {
      producer->produce(cppkafka::MessageBuilder({topic.data(), topic.size()})
                            //.key("null")
                            //.partition(?)
                            .payload({message.data(), message.size()}));
    } catch (std::exception &ex) {
      logger::error(ex.what());
    }

    try {
      // TODO: When to flush?
      producer->flush();
      return true;
    } catch (std::exception &ex) {
      logger::error(ex.what());
    }
    return false;
  }

private:
  // TODO: redis connection pool
  std::unique_ptr<sw::redis::Redis> redisClient;
  std::unique_ptr<cppkafka::Producer> producer;
};
