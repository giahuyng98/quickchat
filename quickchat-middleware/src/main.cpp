#include "channel_service_impl.hpp"
#include "friend_service_impl.hpp"
#include "group_service_impl.hpp"
#include "message_producer_kafka.hpp"
#include "message_service_impl.hpp"
#include "money_service_impl.hpp"
#include "session_service_impl.hpp"
#include "user_service_impl.hpp"

#include "config_json.hpp"
#include "controller.hpp"

int main(int argc, char *argv[]) {

  Controller<UserServiceImpl, FriendServiceImpl, ChannelServiceImpl,
             GroupServiceImpl, MessageServiceImpl, SessionServiceImpl,
             MoneyServiceImpl, MessageProducerKafka>
      controller;

  ConfigJson config;
  config.loadFromFile("config.json");

  controller.start(config);
}
