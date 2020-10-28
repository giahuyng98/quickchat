#ifndef USER_DAO_H
#define USER_DAO_H

#include <mysql++/connection.h>

#include <optional>

#include "gen_id_service_impl.h"
#include "user_info.h"

#define MYSQLPP_SSQLS_NO_STATICS
#include "user.h"

//TODO: GenIdService pass by template
class UserDaoImpl {
public:
  //    template<class Config>
  //    void start(const Config &config) {
  //
  //    }

  template <class Config> void start(const Config &config) {
    GenIdServiceImpl::instance().start(config);

    //model::User::table("users");
    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  std::optional<info::UserInfo> findUserById(const int64_t id);

  std::optional<info::UserInfo> findUserByEmail(const std::string &email);

  std::optional<info::UserInfo> findUserByUsername(const std::string &username);

  std::vector<info::UserInfo> findUser(const std::string &pattern);

  bool add(const info::UserInfo &userInfo);

  bool update(const info::UserInfo &userInfo);

  bool remove(int64_t id);

  std::optional<info::UserInfo> login(const std::string &username,
                                      const std::string &password);

  // friend model::User makeUser(const info::UserInfo &userInfo);
private:
  mysqlpp::Connection conn;
};

#endif
