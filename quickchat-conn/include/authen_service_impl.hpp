#pragma once

#include "authen_service.hpp"
#include "logger.hpp"
#include "user_info.hpp"
#include "util.h"
#include <boost/format.hpp>
#include <memory>
#include <optional>
#include <sw/redis++/redis.h>

namespace detail {
std::string getSessionKey(std::string_view sessionId) {
  return fmt::format("session:{}", sessionId);
}
} // namespace detail

class AuthenServiceImpl : public AuthenService<AuthenServiceImpl> {
private:
  friend class AuthenService<AuthenServiceImpl>;

  template <class Config> void startImpl(const Config &config);

  std::optional<info::UserInfo> authenImpl(std::string_view sessionId);

private:
  std::unique_ptr<sw::redis::Redis> redisClient;
};

std::optional<info::UserInfo>
AuthenServiceImpl::authenImpl(std::string_view sessionId) {
  std::optional<info::UserInfo> result;
  try {
    if (auto userId =
            redisClient->hget(detail::getSessionKey(sessionId), "userId")) {
      result.emplace(info::UserInfo{.id = util::toInt(userId.value())});
    }
  } catch (std::exception &ex) {
  }
  return result;
}

template <class Config>
void AuthenServiceImpl::startImpl(const Config &config) {
  redisClient = std::make_unique<sw::redis::Redis>(
      config["redis"]["uri"].template get<std::string>());
}
