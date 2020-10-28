#pragma once

#include "logger.hpp"
#include "session_service.hpp"
#include "user.pb.h"
#include "util.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <exception>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <sw/redis++/redis.h>

namespace detail {

std::string getSessionId() {
  static boost::uuids::random_generator generator;
  return to_string(generator());
}

std::string getRedisSessionKey(const std::string_view key) {
  return fmt::format("session:{}", key);
};
} // namespace detail

class SessionServiceImpl : public SessionService<SessionServiceImpl> {

  int64_t expiredAfterSecond;
  std::unique_ptr<sw::redis::Redis> redisClient;

public:
  friend class SessionService<SessionServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("session service is started");
    //expiredAfterSecond = config["session"]["expired"].template get<int64_t>();
    redisClient = std::make_unique<sw::redis::Redis>(
        config["redis"]["uri"].template get<std::string>());
  }

  std::optional<std::string> storeImpl(const msg::User &user) {
    throw std::runtime_error("unimplement");
    //const auto sessionId = detail::getSessionId();
    //const auto sessionKey = detail::getRedisSessionKey(sessionId);
    //const auto time =
    //    std::chrono::system_clock::now().time_since_epoch().count();
    //const std::string userId = std::to_string(user.id());
    //
    //// TODO: Pipeline
    //
    //try {
    //  redisClient->hset(sessionKey, "userId", userId);
    //  redisClient->hset(sessionKey, "username", user.username());
    //  redisClient->hset(sessionKey, "fullname", user.fullname());
    //  redisClient->hset(sessionKey, "email", user.email());
    //  redisClient->hset(sessionKey, "time", std::to_string(time));
    //  redisClient->expire(sessionKey, expiredAfterSecond);
    //  return sessionId;
    //} catch (std::exception &ex) {
    //  return {};
    //}
  }

  std::optional<msg::User> authenImpl(const std::string_view sessionId) {
    std::unordered_map<std::string, std::string> buffer;
    try {
      redisClient->hgetall(detail::getRedisSessionKey(sessionId),
                           std::inserter(buffer, buffer.begin()));
      if (buffer.empty()) {
        return {};
      }
      msg::User result;
      result.set_id(util::toInt(buffer["userId"]));
      result.set_username(buffer["username"]);
      result.set_fullname(buffer["fullname"]);
      result.set_email(buffer["email"]);
      return result;
    } catch (std::exception &ex) {
      return {};
    }
  }
};
