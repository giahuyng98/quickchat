#include "config_json.hpp"
#include "friend_service_impl.hpp"
#include "repository_impl.hpp"

int main(int argc, char *argv[]) {
  std::string configFile = "config.json";

  if (argc == 2) {
    configFile = argv[1];
  }

  ConfigJson config;
  config.loadFromFile(configFile);

  FriendServiceImpl<RepositoryImpl> srv;
  srv.start(config);
}
