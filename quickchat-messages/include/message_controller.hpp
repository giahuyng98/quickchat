#pragma once

#include "singleton.hpp"

template <class T>
class MessageController : public Singleton<MessageController<T>> {
public:
  template <class Config> void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }
};
