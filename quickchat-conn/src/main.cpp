#include "channel_service_impl.hpp"
#include "config_json.hpp"
#include "friend_service_impl.hpp"
#include "logger.hpp"
#include "message_producer_kafka.hpp"
#include "message_subscriber_redis.hpp"
#include "session_service_impl.hpp"
#include "websocket_impl.hpp"

template <class WS, class Subscriber> class Application {
public:
  void start(const std::string_view configFile) {

    ConfigJson config;
    config.loadFromFile(configFile);

    Subscriber sub;
    sub.start(config);

    auto &ws = WS::instance();
    ws.start(config);
  }
};

int main(int argc, char *argv[]) {
  logger::setUp();

  std::string configFile = "config.json";
  if (argc == 2) {
    configFile = argv[1];
  }

  using WebSocket = uWebSocket<MessageProducerKafka, SessionServiceImpl,
                               ChannelServiceImpl, FriendServiceImpl>;
  using Subscriber = MessageSubscriberRedis<WebSocket>;

  Application<WebSocket, Subscriber> app;
  app.start(configFile);
}
