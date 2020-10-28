//#include "message.h"
//#include "seen.h"

#include "config_json.hpp"
#include "gen_id_service_impl.h"
#include "message_consumer_impl.hpp"
#include "message_publisher_impl.hpp"
#include "message_repository_impl.hpp"
#include "message_service_impl.hpp"

void sigint_handler(int) {
  //  should_exit.notify_all();
}

template <class MessageService, class Consummer> struct App {
  MessageService messageService;
  Consummer consumer;
  std::condition_variable should_exit;
  std::thread consummerThread;

  void start() {
    ConfigJson config;
    config.loadFromFile("config.json");

    consumer.start(config); // non blocking
    consummerThread = std::thread([&] { consumer.loop(); });
    messageService.start(config); // blocking

    // TODO:  signal hanler
  }
};

int main(int argc, char *argv[]) {

  using MessageRepository = MessageRepositoryImpl<GenIdServiceImpl>;

  using MessageService = MessageServiceImpl<MessageRepository>;

  using MessageConsumer =
      MessageConsumerImpl<MessagePublisherImpl, MessageRepository>;

  App<MessageService, MessageConsumer> app;

  app.start();

  // consummerThread.join();
  // on_signal = [&]() {
  //    dispatcher.stop();
  //};
  //
  // std::mutex mtx;
  // std::unique_lock<std::mutex> l(mtx);
  // should_exit.wait(l);
  //
  return 0;
}
