#pragma once

#include "gen_id_service_impl.h"
#include "user.h"
#include "user.pb.h"
#include "user_repository.hpp"
#include "user_service.pb.h"
#include "util.h"

#include <bcrypt/BCrypt.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <mysql++/connection.h>
#include <mysql++/query.h>
#include <optional>
#include <stdexcept>

namespace detail {

msg::User toProtobufUser(const model::User &user) {
  msg::User result;
  result.set_id(user.id);
  result.set_username(user.username);
  result.set_email(user.email);
  result.set_fullname(user.fullname);
  // result.set_password(user.password);
  return result;
}

std::optional<model::User>
toUserValidated(const uint64_t id, std::string username, std::string email,
                std::string fullname, std::string password) {
  boost::algorithm::all_of(username, boost::algorithm::is_space());
  //if (boost::algorithm::is_any_of(username)) {
  //
  //}

  util::trim(username);
  util::trim(email);
  util::trim(fullname);
  util::trim(password);

  // id, req->username(), req->email(), req->fullname(), password
  // if (user.fullname)
}
} // namespace detail

class UserRepositoryImpl : public UserRepository<UserRepositoryImpl> {
public:
  friend class UserRepository<UserRepositoryImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    model::User::table("users");
    GenIdServiceImpl::instance().start(config);
    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  msg::User getUserImpl(const srv::GetUserRequest *req) {
    msg::User res;
    mysqlpp::Query query = conn.query();
    query << "SELECT id, username, email, fullname, password FROM users WHERE "
             "id = "
          << req->user_id();
    auto queryResult = query.store();
    // TODO: use custom exception
    if (queryResult.empty()) {
      throw std::logic_error("user is not existed");
    }
    res = detail::toProtobufUser(queryResult[0]);
    return res;
  }

  msg::User createUserImpl(const srv::CreateUserRequest *req) {
    msg::User res;
    mysqlpp::Query query = conn.query();

    const uint64_t id = GenIdServiceImpl::instance().nextId();
    const std::string password = BCrypt::generateHash(req->password());

    model::User user(id, req->username(), req->email(), req->fullname(),
                     password);
    query.insert(user);
    query.execute();
    res = detail::toProtobufUser(user);
    return res;
  }

  msg::User updateUserImpl(const srv::UpdateUserRequest *req) {
    msg::User res;
    throw std::runtime_error("Unimplement");
    return res;
  }

  google::protobuf::Empty deleteUserImpl(const srv::DeleteUserRequest *req) {
    throw std::runtime_error("Unimplement");
    return {};
  }

  msg::User loginImpl(const srv::LoginRequest *req) {
    // TODO: refactor
    if (!req->username().empty()) {
      return loginByUsername(req->username(), req->password());
    }

    if (!req->email().empty()) {
      return loginByEmail(req->email(), req->password());
    }
    throw mysqlpp::BadQuery("Missing username or email");
  }

  // TODO: merge 2 function
  msg::User loginByUsername(const std::string &username,
                            const std::string &password) {
    constexpr auto QUERY = "SELECT users.* FROM users WHERE username = %0q";
    mysqlpp::Query query = conn.query(QUERY);
    query.parse();
    const auto res = query.store(username);
    if (res.empty()) {
      throw mysqlpp::BadQuery("Wrong username or password");
    }
    const model::User user = res[0];
    const bool ok = BCrypt::validatePassword(password, user.password);
    if (ok) {
      return detail::toProtobufUser(user);
    } else {
      throw mysqlpp::BadQuery("Wrong username or password");
    }
  }

  msg::User loginByEmail(const std::string &email,
                         const std::string &password) {
    constexpr auto QUERY = "SELECT users.* FROM users WHERE username = %0q";
    mysqlpp::Query query = conn.query(QUERY);
    query.parse();
    const auto res = query.store(email);
    if (res.empty()) {
      throw mysqlpp::BadQuery("Wrong username or password");
    }
    const model::User user = res[0];
    const bool ok = BCrypt::validatePassword(password, user.password);
    if (ok) {
      return detail::toProtobufUser(user);
    } else {
      throw mysqlpp::BadQuery("Wrong username or password");
    }
  }

private:
  mysqlpp::Connection conn;
};
