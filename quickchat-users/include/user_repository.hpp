#pragma once

#include "user_service.pb.h"
template <class T> class UserRepository {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  inline msg::User getUser(const srv::GetUserRequest *req) {
    return static_cast<T &>(*this).getUserImpl(req);
  }

  inline msg::User createUser(const srv::CreateUserRequest *req) {
    return static_cast<T &>(*this).createUserImpl(req);
  }

  inline msg::User updateUser(const srv::UpdateUserRequest *req) {
    return static_cast<T &>(*this).updateUserImpl(req);
  }

  inline google::protobuf::Empty deleteUser(const srv::DeleteUserRequest *req) {
    return static_cast<T &>(*this).deleteUserImpl(req);
  }

  inline msg::User login(const srv::LoginRequest *req) {
    return static_cast<T &>(*this).loginImpl(req);
  }
};
