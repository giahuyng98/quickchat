//
// Created by giahuy on 9/28/20.
//

#ifndef QUICKCHAT_USER_USER_CONTROLLER_HPP
#define QUICKCHAT_USER_USER_CONTROLLER_HPP

#include "logger.hpp"
#include "user_info.h"
#include <mysql++/exceptions.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <uWebSockets/App.h>

#define API_VERSION "/api/v1"

template <class SessionMgr, class UserDao> class UserController {
public:
  template <class Config> void start(const Config &config) {
    sessionMgr.start(config);
    userDao.start(config);

    app.post(API_VERSION "/users/register", registerHandler());
    app.post(API_VERSION "/users/login", loginHandler());

    int port = config["port"].template get<int>();
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
  auto registerHandler();
  auto loginHandler();

private:
  SessionMgr sessionMgr;
  UserDao userDao;
  uWS::App app;
};

template <class SessionMgr, class UserDao>
auto UserController<SessionMgr, UserDao>::registerHandler() {
  return [&](auto *res, auto *req) {
    res->onAborted([=]() {
      res->writeHeader("Content-Type", "application/json");
      logger::error("Aborted");
      res->end(R"()");
    });
    res->onData([&, buff = std::string(), res](std::string_view data,
                                               bool last) mutable {
      buff.append(data.data(), data.size());
      if (last) {
        try {
          const nlohmann::json body = nlohmann::json::parse(buff);
          const bool result = userDao.add(body.get<info::UserInfo>());
          res->writeHeader("Content-Type", "application/json");
          res->end(result ? R"("error":0,"message":"register done","data":"")"
                          : R"("error":1,"message":"fail")");
        } catch (nlohmann::json::parse_error &ex) {
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(R"({"error":1,"message":"format"})");
        } catch (mysqlpp::Exception &ex) {
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(
              nlohmann::json{{"error", "1"}, {"message", ex.what()}}.dump());
        } catch (std::exception &ex) {
          // const auto &ex = std::current_exception();
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(nlohmann::json{{"error", "1"}, {"message", "unknow error"}}
                       .dump());
        }
      }
    });
  };
}

template <class SessionMgr, class UserDao>
auto UserController<SessionMgr, UserDao>::loginHandler() {
  return [&](auto *res, auto *req) {
    res->onAborted([=]() {
      res->writeHeader("Content-Type", "application/json");
      res->end(R"()");
    });
    std::string buffer;
    res->onData([&, buff = std::move(buffer), res](std::string_view data,
                                                   bool last) mutable {
      buff.append(data.data(), data.size());
      if (last) {
        try {
          const nlohmann::json body = nlohmann::json::parse(buff);
          res->writeHeader("Content-Type", "application/json");
          if (const std::optional<info::UserInfo> userInfo =
                  userDao.login(body["username"].get<std::string>(),
                                body["password"].get<std::string>());
              userInfo.has_value()) {
            const auto sessionId = sessionMgr.add(userInfo.value());
            res->end(nlohmann::json{
                {"error", "0"},
                {"message", "Login successfully"},
                {"data",
                 {
                     {"sessionId", sessionId},
                     {"username", userInfo->username},
                     {"email", userInfo->email},
                     {"fullname", userInfo->fullname},
                 }}}.dump());
          } else {
            res->end(nlohmann::json{
                {"error", "0"},
                {"message", "Login fail, wrong username or password"}}
                         .dump());
          }
        } catch (nlohmann::json::parse_error &ex) {
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(R"({"error":1,"message":"format"})");
        } catch (mysqlpp::Exception &ex) {
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(
              nlohmann::json{{"error", "1"}, {"message", ex.what()}}.dump());
        } catch (std::exception &ex) {
          logger::error(ex.what());
          res->writeHeader("Content-Type", "application/json");
          res->end(nlohmann::json{{"error", "1"}, {"message", "unknown error"}}
                       .dump());
        }
      }
    });
  };
}

#endif // QUICKCHAT_USER_USER_CONTROLLER_HPP
