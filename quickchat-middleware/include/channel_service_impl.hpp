#pragma once

#include "channel.pb.h"
#include "channel_service.grpc.pb.h"
#include "channel_service.hpp"
#include "channel_service.pb.h"
#include "logger.hpp"
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status.h>
#include <memory>

class ChannelServiceImpl : public ChannelService<ChannelServiceImpl> {
  std::unique_ptr<srv::ChannelService::Stub> stub;
  std::string apikey;

public:
  friend class ChannelService<ChannelServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("channelservice is started");

    apikey = config["channel-service"]["apikey"].template get<std::string>();
    const std::string channelServiceURI =
        config["channel-service"]["uri"].template get<std::string>();

    stub = srv::ChannelService::NewStub(grpc::CreateChannel(
        channelServiceURI, grpc::InsecureChannelCredentials()));
  }

  std::pair<grpc::Status, msg::Channel>
  getChannelImpl(const uint64_t channelId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::GetChannelRequest req;
    req.set_channel_id(channelId);
    msg::Channel res;
    auto status = stub->GetChannel(&context, req, &res);

    return std::make_pair(status, res);
  }

  auto listUserChannelsImpl(const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::ListUserChannelsRequest req;
    req.set_user_id(userId);

    srv::ListUserChannelsReponse res;
    auto status = stub->ListUserChannels(&context, req, &res);

    return std::make_pair(status, *res.mutable_channels());
  }

  std::pair<grpc::Status, msg::Channel>
  createChannelImpl(const srv::CreateChannelRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    msg::Channel res;
    auto status = stub->CreateChannel(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, srv::CheckIsMemberAndGetChannelReponse>
  checkIsMemberAndGetChannelImpl(const uint64_t channelId,
                                 const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::CheckIsMemberAndGetChannelRequest req;
    req.set_channel_id(channelId);
    req.set_user_id(userId);

    srv::CheckIsMemberAndGetChannelReponse res;
    auto status = stub->CheckIsMemberAndGetChannel(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, srv::CheckIsMemberChannelReponse>
  checkIsMemberImpl(const uint64_t channelId, const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::CheckIsMemberChannelRequest req;
    req.set_channel_id(channelId);
    req.set_user_id(userId);

    srv::CheckIsMemberChannelReponse res;
    auto status = stub->CheckIsMemberChannel(&context, req, &res);

    return std::make_pair(status, res);
  }
  std::pair<grpc::Status, msg::Channel>
  getPrivateChannelImpl(const uint64_t uid1, const uint64_t uid2) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::GetPrivateChannelRequest req;
    req.set_uid1(uid1);
    req.set_uid2(uid2);

    msg::Channel res;
    auto status = stub->GetPrivateChannel(&context, req, &res);

    return std::make_pair(status, res);
  }

  // std::string getChannelNameImpl(const uint64_t channelId) {
  //
  //}
};
