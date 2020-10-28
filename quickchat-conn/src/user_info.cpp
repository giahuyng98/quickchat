#include "user_info.hpp"

namespace info {

std::ostream &operator<<(std::ostream &os, const info::UserInfo &userInfo) {
  return os << nlohmann::json(userInfo);
}
void to_json(nlohmann::json &j, const UserInfo &userInfo) {
  j = nlohmann::json{{"id", userInfo.id},
                     {"username", userInfo.username},
                     {"email", userInfo.email},
                     {"fullname", userInfo.fullname}};
}
void from_json(const nlohmann::json &j, UserInfo &userInfo) {
  // j.at("id").get_to(userInfo.id);
  j.at("username").get_to(userInfo.username);
  j.at("email").get_to(userInfo.email);
  j.at("fullname").get_to(userInfo.fullname);
  j.at("password").get_to(userInfo.password);
}
} // namespace info
