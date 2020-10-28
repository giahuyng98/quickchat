//
// Created by giahuy on 9/28/20.
//

#include "session_mgr_impl.hpp"
#include "logger.hpp"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace detail {

std::string getSessionId() {
  static boost::uuids::random_generator generator;
  return to_string(generator());
}

std::string getRedisSessionKey(const std::string_view key) {
  return fmt::format("session:{}", key);
};
} // namespace detail

std::string SessionMgrImpl::addImpl(const info::UserInfo &userInfo) {
  const auto sessionId = detail::getSessionId();
  const auto sessionKey = detail::getRedisSessionKey(sessionId);
  const auto time = std::chrono::system_clock::now().time_since_epoch().count();

  redisClient->hset(sessionKey, "userId", std::to_string(userInfo.id));
  redisClient->hset(sessionKey, "time", std::to_string(time));
  redisClient->expire(sessionKey, expiredAfterSecond);
  return sessionId;
}

bool SessionMgrImpl::removeImpl(const std::string &sessionId) { assert(false); }

bool SessionMgrImpl::removeImpl(int64_t userId) {
  assert(false);
  return false;
}
