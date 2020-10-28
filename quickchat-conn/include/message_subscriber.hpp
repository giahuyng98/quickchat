#pragma once

template <class Sub, class WS> class MessageSubscriber {
public:
  template <class Config> void start(const Config &config) {
    static_cast<Sub &>(*this).startImpl(config);
  }
};
