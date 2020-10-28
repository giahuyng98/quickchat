#pragma once

#include "channel_service.pb.h"
#include <cstdint>

template <class T> class ChannelService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }

  inline auto getChannel(const uint64_t channelId) {
    return static_cast<T &>(*this).getChannelImpl(channelId);
  }

  inline auto listUserChannels(const uint64_t userId) {
    return static_cast<T &>(*this).listUserChannelsImpl(userId);
  }

  inline auto createChannel(const srv::CreateChannelRequest &req) {
    return static_cast<T &>(*this).createChannelImpl(req);
  }

  inline auto checkIsMember(const uint64_t channelId, const uint64_t userId) {
    return static_cast<T &>(*this).checkIsMemberImpl(channelId, userId);
  }

  inline auto checkIsMemberAndGetChannel(const uint64_t channelId,
                                         const uint64_t userId) {
    return static_cast<T &>(*this).checkIsMemberAndGetChannelImpl(channelId,
                                                                  userId);
  }

  inline auto getPrivateChannel(const uint64_t uid1, const uint64_t uid2) {
    return static_cast<T &>(*this).getPrivateChannelImpl(uid1, uid2);
  }

  // inline auto getChannelName(const uint64_t channelId) {
  //  return static_cast<T &>(*this).getChannelNameImpl(channelId);
  //}
};
