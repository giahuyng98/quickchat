#pragma once

#include "user_info.hpp"
#include "config.hpp"

template <class T> class AuthenService {
public:
  template<class Config>
  void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  std::optional<info::UserInfo> authen(std::string_view sessionId) {
    return static_cast<T &>(*this).authenImpl(sessionId);
  }
};
