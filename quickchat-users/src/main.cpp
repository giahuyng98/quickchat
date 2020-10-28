#include "config_json.hpp"
//#include "session_mgr_impl.hpp"
//#include "user_controller.hpp"
#include "user_repository_impl.hpp"
#include "user_service_impl.hpp"

//#define MYSQLPP_SSQLS_NO_STATICS
//#include "user_dao_impl.h"

// struct App {
//  App() {
//    ConfigJson config;
//    config.loadFromFile("config.json");
//
//    UserController<SessionMgrImpl, UserDaoImpl> controller;
//    controller.start(config);
//  }
//};
//
int main(int argc, char *argv[]) {
  // App();

  ConfigJson config;
  config.loadFromFile("config.json");

  UserServiceImpl<UserRepositoryImpl> userService;
  userService.start(config);
  return 0;
}
