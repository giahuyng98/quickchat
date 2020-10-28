#pragma once

#include "friend_service.pb.h"
#include <cstdint>

template <class T> class FriendService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }

  auto inline listUserFriends(const uint64_t userId) {
    return static_cast<T &>(*this).listUserFriendsImpl(userId);
  }

  auto inline createFriend(const uint64_t from, const uint64_t to) {
    return static_cast<T &>(*this).createFriendImpl(from, to);
  }

  auto inline deleteFriend(const uint64_t from, const uint64_t to) {
    return static_cast<T &>(*this).deleteFriendImpl(from, to);
  }
};
