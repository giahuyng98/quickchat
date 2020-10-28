//
// Created by giahuy on 9/28/20.
//

#ifndef QUICKCHAT_USER_GEN_ID_SERVICE_H
#define QUICKCHAT_USER_GEN_ID_SERVICE_H

#include "singleton.hpp"
#include <cstdint>

template <class T> class GenIdService : public Singleton<T> {

public:
  friend class Singleton<T>;
  GenIdService() = default;
  template <class Config> void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  int64_t nextId() { return static_cast<T &>(*this).nextIdImpl(); }
};

#endif // QUICKCHAT_USER_GEN_ID_SERVICE_H
