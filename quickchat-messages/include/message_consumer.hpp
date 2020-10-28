#pragma once

template <class Self, class Producer> class MessageConsumer {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<Self &>(*this).startImpl(config);
  }

  inline void loop() { return static_cast<Self &>(*this).loopImpl(); }

};
