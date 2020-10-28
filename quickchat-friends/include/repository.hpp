#pragma once

#include "friend_service.pb.h"
template <class T> class Repository {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  auto inline listUserFriends(const ::srv::ListUserFriendsRequest *req) {
    return static_cast<T &>(*this).listUserFriendsImpl(req);
  }

  auto inline createFriend(const ::srv::BaseRequest *req) {
    return static_cast<T &>(*this).createFriendImpl(req);
  }

  auto inline deleteFriend(const ::srv::BaseRequest *req) {
    return static_cast<T &>(*this).deleteFriendImpl(req);
  }

  auto inline changeFriendStatus(const ::srv::ChangeFriendStatusRequest *req) {
    return static_cast<T &>(*this).changeFriendStatusImpl(req);
  }
};
