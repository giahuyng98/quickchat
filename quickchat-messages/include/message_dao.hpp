#pragma once

#include "internal_message.pb.h"
#include <cstdint>
namespace info {
class MessageInfo;
}

template <class Self, class GenIdService> class MessageDao {
public:
  template <class Config> void start(const Config &config) {
    static_cast<Self &>(*this).startImpl(config);
  }
  bool add(const info::MessageInfo &msgInfo) {
    return static_cast<Self &>(*this).addImpl(msgInfo);
  }
  
  uint64_t add(const msg::Chat &chat){
    return static_cast<Self &>(*this).addImpl(chat);
  }

  bool update(const info::MessageInfo &msgInfo) {
    return static_cast<Self &>(*this).updateImpl(msgInfo);
  }

  bool remove(const int64_t id) {
    return static_cast<Self &>(*this).remove(id);
  }
};
