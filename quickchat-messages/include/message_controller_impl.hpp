#pragma once

#include "message_controller.hpp"
#include <uWebSockets/App.h>

class MessageControllerImpl : public MessageController<MessageControllerImpl> {
  friend class MessageController<MessageControllerImpl>;

private:
  template <class Config> void startImpl(const Config &config) {

  }

  uWS::App app;
};
