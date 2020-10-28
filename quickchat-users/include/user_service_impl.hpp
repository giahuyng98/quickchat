#pragma once

#include "logger.hpp"
#include "user_repository_impl.hpp"
#include "user_service.grpc.pb.h"
#include <exception>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace detail {
std::string_view getApiKey(const grpc::ServerContext *context) {
  const auto it = context->client_metadata().find("apikey");
  if (it == context->client_metadata().end()) {
    return {};
  }
  return {it->second.data(), it->second.size()};
}
} // namespace detail

template <class UserRepository>
class UserServiceImpl final : public srv::UserService::Service {

  std::string apikey;
  UserRepository repo;

public:
  template <class Config> void start(const Config &config) {
    repo.start(config);

    apikey = config["rpc"]["apikey"].template get<std::string>();

    grpc::EnableDefaultHealthCheckService(true);
    const std::string address =
        config["rpc"]["uri"].template get<std::string>();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    logger::info("rpc user service listening on {}", address);
    server->Wait();
  }

private:
  bool authen(const std::string_view key) { return this->apikey == key; }

  ::grpc::Status GetUser(::grpc::ServerContext *context,
                         const ::srv::GetUserRequest *request,
                         ::msg::User *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.getUser(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (mysqlpp::BadConversion &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, ex.what());
    } catch (mysqlpp::Exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::INTERNAL, ex.what());
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  grpc::Status CreateUser(::grpc::ServerContext *context,
                          const ::srv::CreateUserRequest *request,
                          ::msg::User *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.createUser(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");

    } catch (mysqlpp::Exception &ex) {
      logger::error(ex.what());

      return grpc::Status(grpc::StatusCode::INTERNAL, ex.what());
    } catch (std::exception &ex) {

      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

public:
  grpc::Status UpdateUser(::grpc::ServerContext *context,
                          const ::srv::UpdateUserRequest *request,
                          ::msg::User *response) override {
    try {

      if (authen(detail::getApiKey(context))) {
        *response = repo.updateUser(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");

    } catch (mysqlpp::Exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::INTERNAL, ex.what());
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  grpc::Status DeleteUser(::grpc::ServerContext *context,
                          const ::srv::DeleteUserRequest *request,
                          ::google::protobuf::Empty *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.deleteUser(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (mysqlpp::Exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::INTERNAL, ex.what());
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  grpc::Status Login(::grpc::ServerContext *context,
                     const ::srv::LoginRequest *request,
                     ::msg::User *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.login(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (mysqlpp::Exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::INTERNAL, ex.what());
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }
};
