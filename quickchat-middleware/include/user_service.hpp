#pragma once

#include "user_service.pb.h"
#include <cstdint>

template <class T> class UserService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }

  inline auto getUser(const uint64_t userId) {
    return static_cast<T &>(*this).getUserImpl(userId);
  }

  inline auto createUser(const srv::CreateUserRequest &req) {
    return static_cast<T &>(*this).createUserImpl(req);
  }

  inline auto login(const srv::LoginRequest &req) {
    return static_cast<T &>(*this).loginImpl(req);
  }
};
