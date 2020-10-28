#include "channel_service_impl.hpp"
#include "config_json.hpp"
#include "repository_impl.hpp"

int main(int argc, char *argv[]) {
    logger::setUp();


  ChannelServiceImpl<RepositoryImpl> channelSrv;

  ConfigJson config;
  config.loadFromFile("config.json");
  channelSrv.start(config);



  return 0;
}
