#pragma once

#include <cstdint>

template <class T> class FriendService {
public:
  template <class Config> void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  auto getOnlineFriends(const uint64_t channelId) {
    return static_cast<T &>(*this).getOnlineFriendsImpl(channelId);
  }
};
