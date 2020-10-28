#pragma once

#include "user.pb.h"
template <class T> class SessionService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }

  auto store(const msg::User &user) {
    return static_cast<T &>(*this).storeImpl(user);
  }

  auto authen(const std::string_view sessionId) {
    return static_cast<T &>(*this).authenImpl(sessionId);
  }

};
