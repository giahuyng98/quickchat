#pragma once

#include "friend_service.grpc.pb.h"
#include "logger.hpp"
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

template <class Repository>
class FriendServiceImpl final : public srv::FriendService::Service {

  Repository repo;
  std::string apikey;

public:
  bool authen(std::string_view key) { return this->apikey == key; }

  template <class Config> void start(const Config &config) {
    repo.start(config);

    grpc::EnableDefaultHealthCheckService(true);
    const std::string address =
        config["rpc"]["uri"].template get<std::string>();

    apikey = config["rpc"]["apikey"].template get<std::string>();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    logger::info("rpc friend service listening on {}", address);
    server->Wait();
  }

  ::grpc::Status
  ListUserFriends(::grpc::ServerContext *context,
                  const ::srv::ListUserFriendsRequest *request,
                  ::srv::ListUserFriendsReponse *response) override {

    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.listUserFriends(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status CreateFriend(::grpc::ServerContext *context,
                              const ::srv::BaseRequest *request,
                              ::srv::BaseReponse *response) override {

    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.createFriend(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }
  ::grpc::Status DeleteFriend(::grpc::ServerContext *context,
                              const ::srv::BaseRequest *request,
                              ::srv::BaseReponse *response) override {

    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.deleteFriend(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }
  ::grpc::Status
  ChangeFriendStatus(::grpc::ServerContext *context,
                     const ::srv::ChangeFriendStatusRequest *request,
                     ::srv::BaseReponse *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.changeFriendStatus(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }
};
