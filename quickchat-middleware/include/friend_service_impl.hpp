#pragma once

#include "friend.pb.h"
#include "friend_service.grpc.pb.h"
#include "friend_service.hpp"
#include "friend_service.pb.h"
#include "logger.hpp"
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status.h>
#include <memory>

class FriendServiceImpl : public FriendService<FriendServiceImpl> {
  std::unique_ptr<srv::FriendService::Stub> stub;
  std::string apikey;

public:
  friend class FriendService<FriendServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("friendservice is started");

    apikey = config["friend-service"]["apikey"].template get<std::string>();
    const std::string friendServiceURI =
        config["friend-service"]["uri"].template get<std::string>();

    stub = srv::FriendService::NewStub(grpc::CreateChannel(
        friendServiceURI, grpc::InsecureChannelCredentials()));
  }

  std::pair<grpc::Status, srv::BaseReponse>
  createFriendImpl(const uint64_t from, const uint64_t to) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::BaseRequest req;
    req.set_from(from);
    req.set_to(to);

    srv::BaseReponse res;
    auto status = stub->CreateFriend(&context, req, &res);
    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, srv::BaseReponse>
  deleteFriendImpl(const uint64_t from, const uint64_t to) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::BaseRequest req;
    req.set_from(from);
    req.set_to(to);

    srv::BaseReponse res;
    auto status = stub->CreateFriend(&context, req, &res);
    return std::make_pair(status, res);
  }
  auto listUserFriendsImpl(const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::ListUserFriendsRequest req;
    req.set_user_id(userId);

    srv::ListUserFriendsReponse res;
    auto status = stub->ListUserFriends(&context, req, &res);
    return std::make_pair(status, res);
  }
};
