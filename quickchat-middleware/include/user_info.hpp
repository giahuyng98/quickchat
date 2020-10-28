#ifndef USER_INFO_H
#define USER_INFO_H

#include <iostream>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace info {
struct UserInfo {
  int64_t id;
  std::string username;
  std::string email;
  std::string fullname;
  std::string password;

  friend std::ostream &operator<<(std::ostream &os, const UserInfo &userInfo);

  // NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfo, id, username, email, fullname,
  // password)
};

void to_json(nlohmann::json &j, const UserInfo &userInfo);
void from_json(const nlohmann::json &j, UserInfo &userInfo);
} // namespace info
#endif
