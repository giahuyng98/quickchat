#pragma once

#include "channel.pb.h"
#include "channel_service.grpc.pb.h"
#include "channel_service.hpp"
#include "channel_service.pb.h"
#include "logger.hpp"
#include "util.h"
#include <algorithm>
#include <fmt/ranges.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status.h>
#include <iterator>
#include <memory>
#include <optional>
#include <spdlog/common.h>
#include <sw/redis++/redis++.h>

namespace detail {
//
// std::string getChannelKey(const uint64_t channelId) {
//  return fmt::format("channel:{}", channelId);
//}

std::string getChannelMembersKey(const uint64_t channelId) {
  return fmt::format("channel:{}:members", channelId);
}

class ChannelCache {
  std::unique_ptr<sw::redis::Redis> cache;

public:
  template <class Config> void start(const Config &config) {
    const std::string redisURI =
        config["redis"]["uri"].template get<std::string>();
    cache = std::make_unique<sw::redis::Redis>(redisURI);
  }

  bool storeChannelMembers(
      const uint64_t channelId,
      const google::protobuf::RepeatedPtrField<::msg::Channel_Member>
          &members) {

    const std::string key = getChannelMembersKey(channelId);

    std::for_each(std::begin(members), std::end(members),
                  [&](const msg::Channel_Member &mem) {
                    cache->sadd(key, std::to_string(mem.user_id()));
                  });
    // TODO: what is this return
    return true;
  }

  auto getChannelMembers(const uint64_t channelId) {
    google::protobuf::RepeatedPtrField<::msg::Channel_Member> result;
    std::vector<std::string> buff;
    // TODO: avoid buff
    cache->smembers(getChannelMembersKey(channelId),
                    std::inserter(buff, buff.begin()));
    std::for_each(std::begin(buff), std::end(buff),
                  [&](const std::string &mem) {
                    result.Add()->set_user_id(util::toInt(mem));
                  });
    return result;
  }
};
} // namespace detail

class ChannelServiceImpl : public ChannelService<ChannelServiceImpl> {

  std::unique_ptr<srv::ChannelService::Stub> stub;
  std::string apikey;

  detail::ChannelCache cache;

public:
  friend class ChannelService<ChannelServiceImpl>;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("channelservice is started");

    cache.start(config);

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

  std::optional<google::protobuf::RepeatedPtrField<::msg::Channel_Member>>
  getMembersChannelImpl(const uint64_t channelId) {
    const auto result = cache.getChannelMembers(channelId);

    if (result.empty()) {
      const auto [status, channel] = getChannel(channelId);
      cache.storeChannelMembers(channelId, channel.members());

      if (!status.ok()) {
        logger::info("error_code: {}", status.error_code());
        logger::info("error_message: {}", status.error_message());
        return std::nullopt;
      }
      return std::make_optional(channel.members());
    } else {
      return std::make_optional(result);
    }
  }
};
