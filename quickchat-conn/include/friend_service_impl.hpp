#pragma once

#include "friend_service.hpp"
#include "logger.hpp"
#include <sw/redis++/redis.h>

class FriendServiceImpl : public FriendService<FriendServiceImpl> {
  friend class FriendService<FriendServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("Dummy FriendService is started");
  }

  std::vector<uint64_t> getOnlineFriendsImpl(const uint64_t channelId) {
    return {1, 2, 3, 4, 5, 6701742190139412480ll, 6704457532829597696ll};
  }

private:
  // TODO: redis connection pool
  // std::unique_ptr<sw::redis::Redis> redisClient;
};
