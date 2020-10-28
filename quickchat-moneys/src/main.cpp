#include "config_json.hpp"
#include "money_service.pb.h"
#include "money_service_impl.hpp"
#include "repository_impl.hpp"

void sigint_handler(int) {
  //  should_exit.notify_all();
}

int main(int argc, char *argv[]) {
  std::string configFile = "config.json";
  if (argc == 2) {
    configFile = argv[1];
  }
  ConfigJson config;
  config.loadFromFile(configFile);

  MoneyServiceImpl<RepositoryImpl> moneyService;
  moneyService.start(config);
}
