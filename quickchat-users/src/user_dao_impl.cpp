#include "user_dao_impl.h"

#include <mysql++/query.h>
#include <mysql++/stadapter.h>

#include <bcrypt/BCrypt.hpp>
#include <exception>
#include <optional>

#include "gen_id_service_impl.h"
#include "user.h"
#include "user_info.h"

info::UserInfo fromUser(const model::User &user) {
  return info::UserInfo{.id = user.id,
                        .username = user.username,
                        .email = user.email,
                        .fullname = user.fullname,
                        .password = user.password};
}

model::User makeUser(const info::UserInfo &userInfo) {
  const auto id = GenIdServiceImpl::instance().nextId();
  const auto password = BCrypt::generateHash(userInfo.password);
  return model::User(id, userInfo.username, userInfo.email, userInfo.fullname,
                     password);
}

std::optional<info::UserInfo> UserDaoImpl::findUserById(const int64_t id) {
  constexpr auto QUERY = "SELECT users.* FROM users WHERE id = %0";
  mysqlpp::Query query = conn.query(QUERY);
  query.parse();
  const auto res = query.store(id);
  std::optional<info::UserInfo> result;
  if (!res.empty()) {
    result.emplace(fromUser(res[0]));
  }
  return result;
}

std::optional<info::UserInfo>
UserDaoImpl::findUserByEmail(const std::string &email) {
  constexpr auto QUERY = "SELECT users.* FROM users WHERE email = %0q";
  mysqlpp::Query query = conn.query(QUERY);
  query.parse();
  const auto res = query.store(email);
  std::optional<info::UserInfo> result;
  if (!res.empty()) {
    result.emplace(fromUser(res[0]));
  }
  return result;
}

std::optional<info::UserInfo>
UserDaoImpl::findUserByUsername(const std::string &username) {
  constexpr auto QUERY = "SELECT users.* FROM users WHERE username = %0q";
  mysqlpp::Query query = conn.query(QUERY);
  query.parse();
  const auto res = query.store(username);
  std::optional<info::UserInfo> result;
  if (!res.empty()) {
    result.emplace(fromUser(res[0]));
  }
  return result;
}

std::vector<info::UserInfo> UserDaoImpl::findUser(const std::string &pattern) {
  throw "unimplement";
}

bool UserDaoImpl::add(const info::UserInfo &userInfo) {
  mysqlpp::Query query = conn.query();
  query.insert(makeUser(userInfo));
  return query.exec();
}

bool UserDaoImpl::remove(const int64_t id) {
  mysqlpp::Query query = conn.query("DELETE FROM users WHERE id = %0");
  query.parse();
  return query.execute(id);
}
bool UserDaoImpl::update(const info::UserInfo &userInfo) {
  throw "unimplement";
}

std::optional<info::UserInfo> UserDaoImpl::login(const std::string &username,
                                                 const std::string &password) {
  auto userInfo = findUserByUsername(username);
  // TODO: timing attack ??
  if (!userInfo.has_value())
    return userInfo;
  const bool ok = BCrypt::validatePassword(password, userInfo->password);
  if (!ok) {
    userInfo = std::make_optional<info::UserInfo>();
  }
  return userInfo;
}
