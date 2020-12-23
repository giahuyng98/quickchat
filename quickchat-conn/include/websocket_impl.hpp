#pragma once

#include "body_parser.hpp"
#include "channel.pb.h"
#include "internal_message.pb.h"
#include "logger.hpp"
#include "message_producer.hpp"
#include "payload.pb.h"
#include "session_mgr_impl.hpp"
#include "user_info.hpp"
#include "websocket.hpp"
#include <algorithm>
#include <google/protobuf/message.h>
#include <iostream>
#include <memory>
#include <numeric>
#include <optional>
#include <spdlog/spdlog.h>
#include <string_view>
#include <uWebSockets/App.h>
#include <uWebSockets/PerMessageDeflate.h>

struct PerSocketData {
  uint64_t userId;
  // std::string sessionId;
};

namespace detail {
using WS = uWS::WebSocket<false, true>;

inline PerSocketData *getSocketData(WS *ws) {
  return static_cast<PerSocketData *>(ws->getUserData());
}
auto getSessionId(std::string_view query) {
  BodyParser parser(query);
  const auto sessionId = parser["sessionId"];
  return sessionId;
}

} // namespace detail

template <class Producer, class SessionService, class ChannelService,
          class FriendService>
class uWebSocket
    : public WebSocket<
          uWebSocket<Producer, SessionService, ChannelService, FriendService>> {

public:
  friend class WebSocket<
      uWebSocket<Producer, SessionService, ChannelService, FriendService>>;
  uWebSocket() = default;

private:
  template <class Config> void startImpl(const Config &config) {

    sessionService.start(config);

    channelService.start(config);

    friendService.start(config);

    messageProducer.start(config);

    app = std::make_unique<uWS::App>();

    app->ws<PerSocketData>(
           "/",
           {.compression = uWS::DISABLED,
            .maxPayloadLength =
                config["websocket"]["maxPayloadLength"].template get<int>(),
            .idleTimeout =
                config["websocket"]["idleTimeout"].template get<int>(),
            .maxBackpressure =
                config["websocket"]["maxBackpressure"].template get<int>(),
            .upgrade = upgradeCallback(),
            .open = openCallback(),
            .message = messageCallback(),
            .close = closeCallback()})
        .listen(
            config["websocket"]["port"].template get<int>(),
            [&](auto *token) {
              if (token) {
                logger::info("[{}] is running on port {}",
                             config["appName"].template get<std::string>(),
                             config["websocket"]["port"].template get<int>());
              } else {
                logger::error("[{}] failed to run on port {}",
                              config["appName"].template get<std::string>(),
                              config["websocket"]["port"].template get<int>());
              }
            })
        .run();
  }

  // send binary message to user
  // from list of websocket session
  bool sendToUser(const uint64_t userId, const std::string_view message) {
    const auto &sessions = sess.getList(userId);

    // TODO: check send for each call
    bool increaseBackPressure =
        std::any_of(std::begin(sessions), std::end(sessions),
                    [&](auto *ws) { return ws->send(message); });
    return increaseBackPressure;
  }

  void broadCastMessageToChannel(const uint64_t channelId,
                                 const google::protobuf::Message &message) {
    //logger::info("broadCast: {} {}", channelId, message.DebugString());
    const auto members = channelService.getMembersChannel(channelId);
    //logger::info("members: ");
    //for(auto it = std::begin(*members); it != std::end(*members); ++it) {
    //  logger::info("{}", it->DebugString());
    //}

    if (members) {
      // TODO: check return value
      const auto binaryMessage = message.SerializeAsString();
      std::for_each(std::begin(*members), std::end(*members),
                    [&](const msg::Channel::Member &mem) {
                      // logger::info("send to user: {}", mem.user_id());
                      sendToUser(mem.user_id(), binaryMessage);
                    });
    } else {
      // ERROR channel service error
      logger::error("can't get member list");
    }
  }

  void handleChatMessageImpl(const std::string_view message) {
    msg::Chat chat;
    if (chat.ParseFromArray(message.data(), message.size())) {
      msg::WebSocketOutgoingMessage out;
      *out.mutable_chat() = std::move(chat);
      // TODO: async?
      broadCastMessageToChannel(out.chat().channel_id(), out);
    } else {
      logger::error("can't parse message");
    }
  }

  void handleSeenMessageImpl(std::string_view message) {
    msg::Seen seen;
    if (seen.ParseFromArray(message.data(), message.size())) {
      msg::WebSocketOutgoingMessage out;
      *out.mutable_seen() = std::move(seen);
      broadCastMessageToChannel(out.seen().channel_id(), out);
    } else {
      logger::error("can't parse message");
    }
  }

  void handleTypingMessageImpl(std::string_view message) {
    msg::WebSocketOutgoingMessage out;
    if (out.ParseFromArray(message.data(), message.size())) {
      broadCastMessageToChannel(out.typing().channel_id(), out);
    } else {
      logger::error("can't parse message");
    }
  }

  void handleStatusMessageImpl(std::string_view message) {
    logger::info("{} : {}", __func__, message);
  }

  void handleFriendNotificationImpl(std::string_view message) {
    logger::info("{} : {}", __func__, message);
  }

  void handleGroupNotificationImpl(std::string_view message) {
    logger::info("{} : {}", __func__, message);
  }

  // TODO: better parser

  auto authen(std::optional<std::string_view> sessionId) {
    return sessionId ? sessionService.authen(sessionId.value()) : std::nullopt;
  }

  // TODO: Upgrade async?
  auto upgradeCallback() {
    return [&](auto *res, auto *req, auto *context) {
      if (auto auth = authen(detail::getSessionId(req->getQuery()));
          !auth.has_value()) {
        res->writeStatus("401");
        res->writeHeader("Content-Type", "application/json");
        res->end(nlohmann::json{{"error", "1"}, {"message", "Login require"}}
                     .dump());
      } else {
        // TODO: headers?
        res->template upgrade<PerSocketData>(
            {.userId = auth->id()}, req->getHeader("sec-websocket-key"),
            req->getHeader("sec-websocket-protocol"),
            req->getHeader("sec-websocket-extensions"), context);
      }
    };
  }

  auto openCallback() {
    return [&](auto *ws) {
      auto *userData = detail::getSocketData(ws);
      sess.store(userData->userId, ws);
    };
  }

  auto messageCallback() {
    return [&](auto *ws, std::string_view message, uWS::OpCode opCode) {
      PerSocketData *userData = detail::getSocketData(ws);

      msg::WebSocketIncomingMessage incoming;
      if (incoming.ParseFromArray(message.data(), message.size())) {

        // const auto outsending = incoming.SerializeAsString();
        const uint64_t userId = userData->userId;
        constexpr size_t maxMessageSize = 65536;

        // logger::info("onmessage: {}", incoming.DebugString());

        switch (incoming.type_case()) {
        case msg::WebSocketIncomingMessage::TypeCase::kChat:
          if (incoming.chat().content().content().empty()) {
            break;
          }
          if (incoming.chat().content().content().size() > maxMessageSize) {
            // TODO: error message
            break;
          }
          incoming.mutable_chat()->set_user_id(userId);
          // TODO: client timestamp, server timestamp
          messageProducer.produceChatMessage(
              incoming.chat().SerializeAsString());
          break;

        case msg::WebSocketIncomingMessage::TypeCase::kSeen:
          incoming.mutable_seen()->set_user_id(userId);
          messageProducer.produceSeenMessage(
              incoming.seen().SerializeAsString());
          break;

        case msg::WebSocketIncomingMessage::TypeCase::kStatus:
          incoming.mutable_status()->set_user_id(userId);
          messageProducer.publishStatus(incoming.status().SerializeAsString());
          break;

        case msg::WebSocketIncomingMessage::TypeCase::kTyping: {
          msg::WebSocketOutgoingMessage out;
          msg::Typing typing;
          typing.set_user_id(userId);
          typing.set_channel_id(incoming.typing().channel_id());
          typing.set_is_typing(incoming.typing().is_typing());
          // typing.PrintDebugString();
          *out.mutable_typing() = std::move(typing);
          messageProducer.publishTyping(out.SerializeAsString());
        } break;

        default:
          logger::error("Invalid ws message");
          break;
        }

        // logger::info(incoming.DebugString());
        // producer.produce(incoming.SerializeAsString());
      } else {
        logger::error("Invalid ws message");
        // TODO: Invalid message, send notification to client
        // ws->send("Invalid");
      }
    };
  }

  auto closeCallback() {
    return [&](auto *ws, int code, std::string_view message) {
      PerSocketData *userData = static_cast<PerSocketData *>(ws->getUserData());
      sess.remove(userData->userId, ws);
    };
  }

private:
  SessionMgrImpl<uint64_t, uWS::WebSocket<false, true> *> sess;
  SessionService sessionService;
  Producer messageProducer;
  ChannelService channelService;
  FriendService friendService;
  std::unique_ptr<uWS::App> app;
};
