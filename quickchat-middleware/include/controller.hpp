#pragma once

#include "account.pb.h"
#include "body_parser.hpp"
#include "channel.pb.h"
#include "channel_service.pb.h"
#include "chat_content.pb.h"
#include "friend.pb.h"
#include "friend_service.pb.h"
#include "internal_message.pb.h"
#include "logger.hpp"
#include "money_service.pb.h"
#include "payload.pb.h"
#include "user.pb.h"
#include "user_service.pb.h"
#include "util.h"
#include <algorithm>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/one_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <cctype>
#include <functional>
#include <google/protobuf/message.h>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>
#include <jwt/jwt.hpp>
#include <memory>
#include <optional>
#include <spdlog/fmt/bundled/core.h>
#include <type_traits>
#include <unordered_set>

#define UWS_HTTPRESPONSE_NO_WRITEMARK
#include <uWebSockets/App.h>

#define API_VERSION "/api/v1"

namespace detail {
// TODO: should it be more efficient
std::string messageToJsonString(const google::protobuf::Message &message,
                                bool printPrimitiFields = true) {
  static google::protobuf::util::JsonPrintOptions options;
  options.always_print_primitive_fields = printPrimitiFields;
  std::string result;
  google::protobuf::util::MessageToJsonString(message, &result, options);
  return result;
}

msg::UserReponse::Data
toUserReponseData(const msg::User &user,
                  const std::string_view sessionId = "") {
  msg::UserReponse::Data result;
  result.set_id(user.id());
  result.set_username(user.username());
  result.set_email(user.email());
  result.set_fullname(user.fullname());

  // TODO: seem wrong
  if (!sessionId.empty()) {
    result.set_session_id({sessionId.data(), sessionId.size()});
  }
  return result;
}

// TODO: fix this
template <class Iter> bool is_duplicated(const Iter first, const Iter last) {
  std::unordered_set<size_t> uset;
  return std::any_of(first, last, [&](const auto &v) {
    return !uset.emplace(std::hash<std::remove_const_t<
                             std::remove_reference_t<decltype(v)>>>{}(v))
                .second;
  });
}
} // namespace detail

namespace std {
template <> struct hash<msg::Channel::Member> {
  std::size_t operator()(const msg::Channel::Member &mem) const noexcept {
    return std::hash<uint64_t>{}(mem.user_id());
  }
};
} // namespace std

template <class UserService, class FriendService, class ChannelService,
          class GroupService, class MessageService, class SessionService,
          class MoneyService, class MessageProducer>

class Controller {

private:
  uWS::App app;
  UserService userService;
  ChannelService channelService;
  MessageService messageService;
  SessionService sessionService;
  MoneyService moneyService;
  MessageProducer producer;
  FriendService friendService;

private:
  std::string baokimIss;
  std::string baokimSecret;

public:
  template <class Config> void start(const Config &config) {

    producer.start(config);
    sessionService.start(config);
    channelService.start(config);
    messageService.start(config);
    userService.start(config);
    moneyService.start(config);
    friendService.start(config);

    baokimIss = config["baokim"]["iss"].template get<std::string>();
    baokimSecret = config["baokim"]["secret"].template get<std::string>();

    // TODO: websocket behavior + res->cork, tryEnd

    app.get(API_VERSION "/users", authenUserRoute());
    app.get(API_VERSION "/users/:id", getUserRoute());

    app.post(API_VERSION "/users/register", registerUserRoute());
    app.post(API_VERSION "/users/login", loginUserRoute());

    app.get(API_VERSION "/channels/:id", getChannelRoute());
    app.post(API_VERSION "/channels", createChannelRoute());

    app.get(API_VERSION "/channels/:id/messages", getChannelMessagesRoute());

    app.get(API_VERSION "/money", getAccountMoneyRoute());
    app.post(API_VERSION "/money", createAccountMoneyRoute());
    app.post(API_VERSION "/money/deposit", depositMoneyRoute());
    app.post(API_VERSION "/money/withdraw", withDrawMoneyRoute());
    app.post(API_VERSION "/money/transfer", transferMoneyRoute());
    app.get(API_VERSION "/money/history", historyMoneyRoute());
    app.post(API_VERSION "/money/transfer-baokim", transferMoneyBaokimRoute());
    app.get(API_VERSION "/money/baokim/token", getBaokimTokenRoute());

    app.get(API_VERSION "/friends", getUserFriendsRoute());
    app.post(API_VERSION "/friends", createFriendRoute());
    app.del(API_VERSION "/friends", deleteFriendRoute());

    app.get(API_VERSION "/chat-list", getChatListRoute());

    app.any("/*", invalidRoute());

    const int port = config["port"].template get<int>();

    app.listen(port, [&](auto *token) {
      if (token) {
        logger::info("running on port: {}", port);
      } else {
        logger::error("fail to listen on port: {}", port);
      }
    });
    app.run();
  }

private:
  auto authenUserRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      msg::UserReponse userReponse;

      if (!sessionId) {
        userReponse.set_error(1);
        userReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(userReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);

      if (!auth) {
        userReponse.set_error(1);
        userReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(userReponse));
        return;
      }

      userReponse.set_error(0);
      userReponse.set_message("ok");
      *userReponse.mutable_data() =
          detail::toUserReponseData(*auth, *sessionId);
      res->end(detail::messageToJsonString(userReponse));
      return;
    };
  }

  auto getChannelRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];
      const uint64_t channelId = util::toInt(req->getParameter(0));

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }

      const auto auth = sessionService.authen(*sessionId);

      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      auto [status, reponse] =
          channelService.checkIsMemberAndGetChannel(channelId, auth->id());

      msg::ChannelReponse payload;
      if (!status.ok()) {
        payload.set_error(status.error_code());
        payload.set_message(status.error_message());
        res->end(detail::messageToJsonString(payload));
        return;
      }

      // TODO: std::move?
      if (reponse.is_in()) {
        payload.set_error(0);
        payload.set_message("ok");
        *payload.mutable_data() = reponse.channel();
      } else {
        payload.set_error(401);
        payload.set_message("Not a member");
      }
      res->end(detail::messageToJsonString(payload));
    };
  }

  auto createChannelRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::ChannelReponse channelReponse;

      if (!sessionId) {
        channelReponse.set_error(1);
        channelReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(channelReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        channelReponse.set_error(1);
        channelReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(channelReponse));
        return;
      }

      const uint64_t creatorId = auth->id();

      res->onData([buff = std::string(), aborted, res, creatorId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::CreateChannelRequest createRequest;
        if (!google::protobuf::util::JsonStringToMessage(buff, &createRequest)
                 .ok()) {
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }

        if (createRequest.channel().type() == "private") {
          if (const auto [ok, message] =
                  validatePrivateChannel(createRequest, creatorId);
              ok) {
            const msg::ChannelReponse reponse = createChannelRPC(createRequest);
            res->end(detail::messageToJsonString(reponse));
            return;
          } else {
            res->end(message);
            return;
          }
        } else if (createRequest.channel().type() == "group") {
          if (const auto [ok, message] =
                  validateGroupChannel(createRequest, creatorId);
              ok) {
            const msg::ChannelReponse reponse = createChannelRPC(createRequest);
            res->end(detail::messageToJsonString(reponse));
            return;
          } else {
            res->end(message);
            return;
          }
        } else {
          res->end(R"({"error":"1","message":"invalid group type"})");
          return;
        }
      });
    };
  }

  auto getUserRoute() {
    return [&](auto *res, auto *req) {
      const uint64_t userId = util::toInt(req->getParameter(0));
      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      msg::UserReponse userReponse;
      // TODO: session first
      const auto [status, user] = userService.getUser(userId);
      // TODO: std::move
      *userReponse.mutable_data() = detail::toUserReponseData(user);
      res->end(detail::messageToJsonString(userReponse));
    };
  }
  auto registerUserRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);
      res->onAborted([=]() { *aborted = true; });

      res->onData([&, buff = std::string(), res, aborted](std::string_view data,
                                                          bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());
        if (!last) {
          return;
        }

        res->writeHeader("Content-Type", "application/json");
        res->writeHeader("Access-Control-Allow-Origin", "*");

        srv::CreateUserRequest createRequest;
        auto convertedStatus =
            google::protobuf::util::JsonStringToMessage(buff, &createRequest);

        if (!convertedStatus.ok()) {
          logger::error(convertedStatus.error_message().as_string());
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }

        const auto [rpcStatus, user] = userService.createUser(createRequest);
        if (!rpcStatus.ok()) {
          res->end(fmt::format(R"({{"error":{},"message":"{}"}})",
                               rpcStatus.error_code(),
                               rpcStatus.error_message()));
          return;
        }

        // auto sessionId = sessionService.store(user);
        // if (!sessionId) {
        //  res->end(R"({"error":"1","message":"can't store session"})");
        //  return;
        //}
        msg::UserReponse userReponse;
        userReponse.set_error(0);
        userReponse.set_message("ok");
        *userReponse.mutable_data() = detail::toUserReponseData(user, "");
        res->end(detail::messageToJsonString(userReponse));
      });
    };
  }
  auto loginUserRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);
      res->onAborted([=]() { *aborted = true; });

      res->onData([&, buff = std::string(), res, aborted](std::string_view data,
                                                          bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());
        if (!last) {
          return;
        }

        res->writeHeader("Content-Type", "application/json");
        res->writeHeader("Access-Control-Allow-Origin", "*");

        srv::LoginRequest loginRequest;
        auto convertedStatus =
            google::protobuf::util::JsonStringToMessage(buff, &loginRequest);

        if (!convertedStatus.ok()) {
          logger::error(convertedStatus.error_message().as_string());
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }

        const auto [rpcStatus, user] = userService.login(loginRequest);
        if (!rpcStatus.ok()) {
          res->end(fmt::format(R"({{"error":{},"message":"{}"}})",
                               rpcStatus.error_code(),
                               rpcStatus.error_message()));
          return;
        }

        auto sessionId = sessionService.store(user);
        if (!sessionId) {
          res->end(R"({"error":"1","message":"can't store session"})");
          return;
        }
        msg::UserReponse userReponse;
        userReponse.set_error(0);
        userReponse.set_message("ok");
        *userReponse.mutable_data() =
            detail::toUserReponseData(user, *sessionId);
        res->end(detail::messageToJsonString(userReponse));
      });
    };
  }

  std::string getPrivateChannelName(const msg::Channel &channel,
                                    const uint64_t userId) {

    const auto notThisUser = [&](const msg::Channel::Member &member) {
      return member.user_id() != userId;
    };

    const auto otherUserIter =
        std::find_if(std::begin(channel.members()), std::end(channel.members()),
                     notThisUser);
    if (otherUserIter == std::end(channel.members())) {
      logger::error("private channel has 1 user!!");
      return ":Deleted";
    }
    const auto [status, user] = userService.getUser(otherUserIter->user_id());
    // TODO: status?
    return user.fullname();
  }

  auto getChatListRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      auto [status, channels] = channelService.listUserChannels(auth->id());
      msg::ChatList chatList;

      chatList.set_error(status.error_code());
      chatList.set_message(status.error_message());

      for (msg::Channel &chan : channels) {
        msg::ChatItem chatItem;
        // Private channel doesn't have name,
        // so go and fetch the name
        if (chan.type() == "private") {
          chan.set_name(getPrivateChannelName(chan, auth->id()));
        }

        auto [messageStatus, message] =
            messageService.getLastMessage(chan.id());

        if (messageStatus.ok()) {
          *chatItem.mutable_last_message() = std::move(message);
        } else {
          logger::info(messageStatus.error_code());
          logger::info(messageStatus.error_message());
        }
        *chatItem.mutable_channel() = std::move(chan);

        chatList.mutable_data()->Add(std::move(chatItem));
      }
      res->end(detail::messageToJsonString(chatList));
    };
  }
  auto getChannelMessagesRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }
      const auto auth = sessionService.authen(*sessionId);
      const auto channelId = util::toInt(req->getParameter(0));

      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      const auto [channelStatus, channelRes] =
          channelService.checkIsMember(channelId, auth->id());

      if (!channelStatus.ok()) {
        res->writeStatus("500");
        res->end(R"({"error":1,"message":"Server error"})");
        return;
      }

      if (!channelRes.is_in()) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Permission denied"})");
        return;
      }

      const auto [status, messages] = messageService.getMessages(channelId);

      msg::MessageList messageList;
      if (!status.ok()) {
        messageList.set_error(status.error_code());
        messageList.set_message(status.error_message());
        res->end(detail::messageToJsonString(messageList));
        return;
      }

      *messageList.mutable_data() = std::move(messages);
      res->end(detail::messageToJsonString(messageList));
    };
  }

  // money route
  auto getAccountMoneyRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      const auto [status, account] = moneyService.getAccount(auth->id());

      msg::AccountReponse response;

      if (!status.ok()) {
        response.set_error(status.error_code());
        response.set_message(status.error_message());
        res->end(detail::messageToJsonString(response));
        return;
      }
      *response.mutable_data() = std::move(account);
      res->end(detail::messageToJsonString(response));
    };
  }

  auto createAccountMoneyRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::CreateAccountRequest createRequest;
        // if (!google::protobuf::util::JsonStringToMessage(buff,
        // &createRequest)
        //         .ok()) {
        //  res->end(R"({"error":"1","message":"invalid request"})");
        //  return;
        //}
        createRequest.set_user_id(userId);
        // TODO: fix this
        createRequest.set_type("normal");

        const auto [status, account] =
            moneyService.createAccount(createRequest);

        msg::AccountReponse response;

        if (!status.ok()) {
          response.set_error(status.error_code());
          response.set_message(status.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }

        response.set_message("account created");
        *response.mutable_data() = std::move(account);
        res->end(detail::messageToJsonString(response));
      });
    };
  }

  auto depositMoneyRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::DepositRequest depositRequest;
        if (!google::protobuf::util::JsonStringToMessage(buff, &depositRequest)
                 .ok()) {
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }
        if (!util::is_uuid(depositRequest.id())) {
          res->end(
              R"({"error":"1","message":"request id is not in uuid format"})");
          return;
        }

        depositRequest.set_user_id(userId);

        const auto [status, account] = moneyService.deposit(depositRequest);

        msg::AccountReponse response;

        if (!status.ok()) {
          response.set_error(status.error_code());
          response.set_message(status.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }

        response.set_message("account created");
        *response.mutable_data() = std::move(account);
        res->end(detail::messageToJsonString(response));
      });
    };
  }

  auto withDrawMoneyRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::WithDrawRequest withDrawRequest;
        if (!google::protobuf::util::JsonStringToMessage(buff, &withDrawRequest)
                 .ok()) {
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }
        if (!util::is_uuid(withDrawRequest.id())) {
          res->end(
              R"({"error":"1","message":"request id is not in uuid format"})");
          return;
        }

        withDrawRequest.set_user_id(userId);

        const auto [status, account] = moneyService.withDraw(withDrawRequest);

        msg::AccountReponse response;

        if (!status.ok()) {
          response.set_error(status.error_code());
          response.set_message(status.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }

        response.set_message("account created");
        *response.mutable_data() = std::move(account);
        res->end(detail::messageToJsonString(response));
      });
    };
  }
  auto getBaokimTokenRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      using namespace jwt::params;
      const auto post = parser["form_params"];
      if (post) {
        const auto amount = parser["amount"];
        const auto to_user = parser["to_user"];
        const auto description = parser["description"];

        if (!amount || !to_user || !description) {
          res->writeStatus("400");
          res->end(R"({"error":1,"message":"Missing data"})");
          return;
        }

        const uint64_t now =
            std::chrono::system_clock::now().time_since_epoch().count();

        jwt::jwt_object obj{algorithm("HS256"), secret(baokimSecret),
                            payload({
                                {"iss", baokimIss},
                            })};
        nlohmann::json form_params{
            {"amount", util::toInt(*amount)},
            {"to_user", *to_user},
            {"description", *description},
        };

        obj.add_claim("iat", now / 1000);
        obj.add_claim("nbf", now);
        obj.add_claim("exp", now + 50 * 1000);
        obj.add_claim("form_params", form_params);

        res->end(fmt::format(R"({{"error":"0","message":"ok","data":"{}"}})",
                             obj.signature()));
      } else {
        jwt::jwt_object obj{algorithm("HS256"), secret(baokimSecret),
                            payload({
                                {"iss", baokimIss},
                            })};
        const uint64_t now =
            std::chrono::system_clock::now().time_since_epoch().count();
        obj.add_claim("iat", now / 1000);
        obj.add_claim("nbf", now);
        obj.add_claim("exp", now + 50 * 1000);
        res->end(fmt::format(R"({{"error":"0","message":"ok","data":"{}"}})",
                             obj.signature()));
        res->end("ok");
      }
    };
  }
  auto transferMoneyBaokimRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::TransferRequest transferRequest;
        if (!google::protobuf::util::JsonStringToMessage(buff, &transferRequest)
                 .ok()) {
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }
        if (!util::is_uuid(transferRequest.id())) {
          res->end(
              R"({"error":"1","message":"request id is not in uuid format"})");
          return;
        }

        // TODO: check user exists
        transferRequest.set_user_id(userId);

        if (transferRequest.user_id() == transferRequest.to_user()) {
          res->end(
              R"({"error":"1","message":"can't transfer money to yourself"})");
          return;
        }

        const auto [channelStatus, channel] =
            channelService.getPrivateChannel(userId, transferRequest.to_user());

        msg::AccountReponse response;

        if (!channelStatus.ok()) {
          response.set_error(channelStatus.error_code());
          response.set_message(channelStatus.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }
        //
        // const auto [moneyStatus, account] =
        //    moneyService.transfer(transferRequest);
        //
        // if (!moneyStatus.ok()) {
        //  response.set_error(moneyStatus.error_code());
        //  response.set_message(moneyStatus.error_message());
        //  res->end(detail::messageToJsonString(response));
        //  return;
        //}
        msg::Account account;
        account.set_user_id(userId);

        logger::info(transferRequest.DebugString());
        logger::info(channel.DebugString());

        msg::Chat chat;
        chat.set_channel_id(channel.id());
        chat.set_user_id(userId);
        chat.mutable_content()->set_type(
            msg::ChatContent_ChatContentType_MONEY);
        *chat.mutable_timestamp() =
            google::protobuf::util::TimeUtil::GetCurrentTime();
        chat.mutable_content()->set_content(
            fmt::format("{},{},{}", "baokim", transferRequest.amount(),
                        transferRequest.message()));

        producer.produceChatMessage(chat.SerializeAsString());

        response.set_message("transfer done");
        *response.mutable_data() = std::move(account);
        res->end(detail::messageToJsonString(response));
      });
    };
  }
  auto transferMoneyRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::TransferRequest transferRequest;
        if (!google::protobuf::util::JsonStringToMessage(buff, &transferRequest)
                 .ok()) {
          res->end(R"({"error":"1","message":"invalid request"})");
          return;
        }
        if (!util::is_uuid(transferRequest.id())) {
          res->end(
              R"({"error":"1","message":"request id is not in uuid format"})");
          return;
        }

        // TODO: check user exists
        transferRequest.set_user_id(userId);

        if (transferRequest.user_id() == transferRequest.to_user()) {
          res->end(
              R"({"error":"1","message":"can't transfer money to yourself"})");
          return;
        }

        const auto [channelStatus, channel] =
            channelService.getPrivateChannel(userId, transferRequest.to_user());

        msg::AccountReponse response;

        if (!channelStatus.ok()) {
          response.set_error(channelStatus.error_code());
          response.set_message(channelStatus.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }

        const auto [moneyStatus, account] =
            moneyService.transfer(transferRequest);

        if (!moneyStatus.ok()) {
          response.set_error(moneyStatus.error_code());
          response.set_message(moneyStatus.error_message());
          res->end(detail::messageToJsonString(response));
          return;
        }

        logger::info(transferRequest.DebugString());
        logger::info(channel.DebugString());

        msg::Chat chat;
        chat.set_channel_id(channel.id());
        chat.set_user_id(userId);
        chat.mutable_content()->set_type(
            msg::ChatContent_ChatContentType_MONEY);
        *chat.mutable_timestamp() =
            google::protobuf::util::TimeUtil::GetCurrentTime();
        chat.mutable_content()->set_content(fmt::format(
            "{},{}", transferRequest.amount(), transferRequest.message()));

        producer.produceChatMessage(chat.SerializeAsString());

        response.set_message("transfer done");
        *response.mutable_data() = std::move(account);
        res->end(detail::messageToJsonString(response));
      });
    };
  }

  auto historyMoneyRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }

      constexpr std::string_view defaultLimit = "100";
      constexpr std::string_view defaultOffset = "0";

      const int limit = util::toInt(parser["limit"].value_or(defaultLimit));
      const int offset = util::toInt(parser["offset"].value_or(defaultOffset));

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      auto [status, histories] =
          moneyService.history(auth->id(), limit, offset);

      msg::HistoryReponse response;

      if (!status.ok()) {
        response.set_error(status.error_code());
        response.set_message(status.error_message());
        res->end(detail::messageToJsonString(response));
        return;
      }
      response.set_message("ok");
      *response.mutable_data() = std::move(*histories.mutable_histories());
      res->end(detail::messageToJsonString(response));
    };
  }

  // Friend service
  auto getUserFriendsRoute() {
    return [&](auto *res, auto *req) {
      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      if (!sessionId) {
        res->writeStatus("400");
        res->end(R"({"error":1,"message":"Expect sessionId"})");
        return;
      }
      const auto auth = sessionService.authen(*sessionId);

      if (!auth) {
        res->writeStatus("401");
        res->end(R"({"error":1,"message":"Authentication fail"})");
        return;
      }

      auto [friendStatus, friends] = friendService.listUserFriends(auth->id());

      msg::FriendListReponse response;
      if (!friendStatus.ok()) {
        // res->writeStatus("500");
        response.set_error(friendStatus.error_code());
        response.set_message(friendStatus.error_message());
        res->end(detail::messageToJsonString(response));
        return;
      }

      response.set_message("ok");
      *response.mutable_data()->mutable_friends() =
          std::move(friends.friends());
      *response.mutable_data()->mutable_sent_requests() =
          std::move(friends.sent_requests());
      *response.mutable_data()->mutable_received_request() =
          std::move(friends.received_request());

      // TODO: important refactor these
      auto fillInfo = [&](msg::Friend &f) {
        auto [userStatus, user] = userService.getUser(f.user_id());
        if (userStatus.ok()) {
          f.set_username(user.username());
          f.set_email(user.email());
          f.set_fullname(user.fullname());
        }
      };

      for (msg::Friend &f : *response.mutable_data()->mutable_friends()) {
        fillInfo(f);
      }

      for (msg::Friend &f : *response.mutable_data()->mutable_sent_requests()) {
        fillInfo(f);
      }

      for (msg::Friend &f :
           *response.mutable_data()->mutable_received_request()) {
        fillInfo(f);
      }

      res->end(detail::messageToJsonString(response));
    };
  }

  auto createFriendRoute() {
    return [&](auto *res, auto *req) {
      std::shared_ptr<bool> aborted = std::make_shared<bool>(false);

      res->onAborted([=]() { *aborted = true; });

      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");

      BodyParser parser(req->getQuery());
      const auto sessionId = parser["sessionId"];

      msg::AccountReponse accountReponse;

      if (!sessionId) {
        accountReponse.set_error(1);
        accountReponse.set_message("Expected sessionId");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const auto auth = sessionService.authen(*sessionId);
      if (!auth) {
        accountReponse.set_error(1);
        accountReponse.set_message("Auth fail");
        res->end(detail::messageToJsonString(accountReponse));
        return;
      }

      const uint64_t userId = auth->id();

      res->onData([buff = std::string(), aborted, res, userId,
                   this](std::string_view data, bool last) mutable {
        if (*aborted) {
          return;
        }

        buff.append(data.data(), data.size());

        if (!last) {
          return;
        }

        srv::BaseRequest requestData;
        msg::FriendReponse friendReponse;

        if (!google::protobuf::util::JsonStringToMessage(buff, &requestData)
                 .ok()) {
          friendReponse.set_error(1);
          friendReponse.set_message("invalid data");
          res->end(detail::messageToJsonString(friendReponse));
          return;
        }

        if (requestData.to() == 0) {
          friendReponse.set_error(1);
          friendReponse.set_message("invalid data");
          res->end(detail::messageToJsonString(friendReponse));
          return;
        }

        if (!isUserExistsRPC(requestData.to())) {
          friendReponse.set_error(1);
          friendReponse.set_message("User is not exists");
          res->end(detail::messageToJsonString(friendReponse));
          return;
        }

        if (requestData.to() == userId) {
          friendReponse.set_error(1);
          friendReponse.set_message("Can't friend yourself");
          res->end(detail::messageToJsonString(friendReponse));
          return;
        }

        auto [status, friendData] =
            friendService.createFriend(userId, requestData.to());

        if (!status.ok()) {
          friendReponse.set_error(status.error_code());
          friendReponse.set_message(status.error_message());
          res->end(detail::messageToJsonString(friendReponse));
          return;
        }

        friendReponse.set_error(status.error_code());
        friendReponse.set_message(friendData.message());
        res->end(detail::messageToJsonString(friendReponse));
      });
    };
  }

  auto deleteFriendRoute() {
    return [&](auto *res, auto *req) {};
  }

  auto invalidRoute() {
    return [&](auto *res, auto *req) {
      res->writeHeader("Content-Type", "application/json");
      res->writeHeader("Access-Control-Allow-Origin", "*");
      res->writeStatus("404");
      logger::error("Invalid route | URL: {}, Method: {}", req->getUrl(),
                    req->getMethod());
      res->end(R"({"error":1,"message":"Invalid request"})");
    };
  }
  auto isUserExistsRPC(const uint64_t userId) {
    // TODO: custom rpc method
    const auto [status, user] = userService.getUser(userId);
    return status.ok() && user.id();
  };

  /*{ok, message}*/
  std::pair<bool, std::string>
  validatePrivateChannel(const srv::CreateChannelRequest &request,
                         const uint64_t creatorId) {
    using namespace boost::algorithm;
    const auto &members = request.channel().members();

    if (members.size() != 2) {
      return {false,
              fmt::format(R"({{"error":1,"message":"{}"}})",
                          "private channel must have exactly 2 members.")};
    }
    // if (boost::algorithm::)
    if (!any_of(members, [creatorId](const msg::Channel::Member &mem) {
          return mem.user_id() == creatorId;
        })) {
      return {false,
              fmt::format(R"({{"error":1,"message":"{}"}})",
                          "permission denied: you can't create a channel "
                          "that is not include you!")};
    }

    if (detail::is_duplicated(std::begin(members), std::end(members))) {
      return {false, fmt::format(R"({{"error":1,"message":"{}"}})",
                                 "duplicated users")};
    }

    if (any_of(members, [&](const msg::Channel::Member &mem) {
          return !isUserExistsRPC(mem.user_id());
        })) {
      // TODO: maybe return that one
      return {false, fmt::format(R"({{"error":1,"message":"{}"}})",
                                 "some users are not exists")};
    } else {
      return {true, "ok"};
    }
  };

  std::pair<bool, std::string>
  validateGroupChannel(const srv::CreateChannelRequest &request,
                       const uint64_t creatorId) {
    using namespace boost::algorithm;
    const auto &members = request.channel().members();

    constexpr int MAX_GROUP_SIZE = 10'000;

    if (members.size() > MAX_GROUP_SIZE) {
      std::string msg = fmt::format("can't create group larger than {} members",
                                    MAX_GROUP_SIZE);
      return {false, fmt::format(R"({{"error":1,"message":"{}"}})", msg)};
    }

    if (members.size() <= 1) {
      return {false,
              fmt::format(R"({{"error":1,"message":"{}"}})",
                          "group channel must have larger than 1 members.")};
    }

    if (members[0].user_id() != creatorId) {
      return {false,
              fmt::format(R"({{"error":1,"message":"{}"}})",
                          "creator must be the first members on the list")};
    }
    // if (boost::algorithm::)
    if (!any_of(members, [creatorId](const msg::Channel::Member &mem) {
          return mem.user_id() == creatorId;
        })) {
      return {false,
              fmt::format(R"({{"error":1,"message":"{}"}})",
                          "permission denied: you can't create a channel "
                          "that is not include you!")};
    }

    if (detail::is_duplicated(std::begin(members), std::end(members))) {
      return {false, fmt::format(R"({{"error":1,"message":"{}"}})",
                                 "duplicated users")};
    }

    if (any_of(members, [&](const msg::Channel::Member &mem) {
          return !isUserExistsRPC(mem.user_id());
        })) {
      // TODO: maybe return that one
      return {false, fmt::format(R"({{"error":1,"message":"{}"}})",
                                 "some users are not exists")};
    } else {
      return {true, "ok"};
    }
  };

  msg::ChannelReponse
  createChannelRPC(const srv::CreateChannelRequest &request) {
    msg::ChannelReponse channelReponse;
    const auto [status, channel] = channelService.createChannel(request);
    if (status.ok()) {
      channelReponse.set_error(0);
      channelReponse.set_message("ok");
      // TODO: std::move?
      *channelReponse.mutable_data() = channel;
    } else {
      channelReponse.set_error(status.error_code());
      channelReponse.set_message(status.error_message());
    }
    return channelReponse;
  };
};
