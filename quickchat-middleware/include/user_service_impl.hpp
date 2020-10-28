#pragma once

#include "logger.hpp"
#include "user.pb.h"
#include "user_service.grpc.pb.h"
#include "user_service.hpp"
#include "user_service.pb.h"
#include <grpcpp/grpcpp.h>

class UserServiceImpl : public UserService<UserServiceImpl> {

  std::unique_ptr<srv::UserService::Stub> stub;
  std::string apikey;

public:
  friend class UserService<UserServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {

    logger::info("userService is started");

    const std::string userServiceURI =
        config["user-service"]["uri"].template get<std::string>();
    apikey = config["user-service"]["apikey"].template get<std::string>();

    stub = srv::UserService::NewStub(grpc::CreateChannel(
        userServiceURI, grpc::InsecureChannelCredentials()));
  }

  std::pair<grpc::Status, msg::User> getUserImpl(const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    srv::GetUserRequest req;
    req.set_user_id(userId);

    msg::User res;
    auto status = stub->GetUser(&context, req, &res);

    return {status, res};
  }

  std::pair<grpc::Status, msg::User>
  createUserImpl(const srv::CreateUserRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    msg::User res;
    auto status = stub->CreateUser(&context, req, &res);
    return {status, res};
  }

  std::pair<grpc::Status, msg::User> loginImpl(const srv::LoginRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    msg::User res;
    auto status = stub->Login(&context, req, &res);

    return {status, res};
  }
};
