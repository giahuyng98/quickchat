#pragma once
#include "logger.hpp"
#include "message_subscriber.hpp"
#include <algorithm>
#include <fmt/ranges.h>
#include <iterator>
#include <string_view>
#include <sw/redis++/redis.h>
#include <sw/redis++/subscriber.h>
#include <thread>
#include <unordered_map>

template <class WS>
class MessageSubscriberRedis
    : public MessageSubscriber<MessageSubscriberRedis<WS>, WS> {
public:
  friend class MessageSubscriber<MessageSubscriberRedis<WS>, WS>;
  MessageSubscriberRedis() = default;

private:
  // Stupid mapping table
  // TODO: make it less stupid
  enum class ChannelType : uint8_t {
    Chat,
    Seen,
    Status,
    Typing,
    Friend,
    Group,
    Count
  };
  static constexpr size_t ChannelTypeSize =
      static_cast<size_t>(ChannelType::Count);

  static constexpr std::array<std::pair<std::string_view, ChannelType>,
                              ChannelTypeSize>
      ChannelTable = {{
          {"chat", ChannelType::Chat},
          {"seen", ChannelType::Seen},
          {"status", ChannelType::Status},
          {"typing", ChannelType::Typing},
          {"friend", ChannelType::Friend},
          {"group", ChannelType::Group},
      }};
  static constexpr ChannelType getChannelType(const std::string_view channel) {
    const auto it =
        std::find_if(ChannelTable.begin(), ChannelTable.end(),
                     [&](const auto &p) { return p.first == channel; });
    return it == ChannelTable.end() ? ChannelType::Count : it->second;
  }
  // End mapping table

  sw::redis::Subscriber createSubscriber(const std::string &uri) {
    return sw::redis::Redis(uri).subscriber();
  }

  inline auto onMessage() {
    return [&](const std::string &pattern, const std::string &channel,
               const std::string &message) {
      const size_t stripPrefix = pattern.length() - 1;

      // TODO: can be optimize or map<string, function> ?
      const auto suff = channel.substr(stripPrefix);

      switch (getChannelType(suff)) {
      case ChannelType::Chat:
        WS::instance().handleChatMessage(message);
        break;
      case ChannelType::Seen:
        WS::instance().handleSeenMessage(message);
        break;
      case ChannelType::Status:
        WS::instance().handleStatusMessage(message);
        break;
      case ChannelType::Typing:
        WS::instance().handleTypingMessage(message);
        break;
      case ChannelType::Friend:
        WS::instance().handleFriendNotification(message);
        break;
      case ChannelType::Group:
        WS::instance().handleGroupNotification(message);
        break;

      default:
        logger::error("redis receive message from unknown channel: {}",
                      channel);
        logger::error("message: {}", message);
        break;
      };
    };
  }

  template <class Config> void startImpl(const Config &config) {
    // TODO: redis connection pool
    auto sub =
        createSubscriber(config["redis"]["uri"].template get<std::string>());

    sub.on_pmessage(onMessage());

    const auto channels = config["redis"]["subscriber"]["channels"]
                              .template get<std::vector<std::string>>();
    logger::info("redis psubscribe: => {}", channels);

    sub.psubscribe(std::begin(channels), std::end(channels));

    auto redisLoop = [](auto &&sub) {
      logger::info("redis start loop");
      while (true) {
        try {
          sub.consume();
        } catch (const sw::redis::TimeoutError &e) {
          continue;
        } catch (const sw::redis::Error &e) {
          logger::error(e.what());
        }
      }
    };
    redisThread = std::thread(redisLoop, std::move(sub));
    // TODO: Join redisThread
  }

private:
  std::thread redisThread;
};
