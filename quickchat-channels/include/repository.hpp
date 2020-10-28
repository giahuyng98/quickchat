#pragma once

namespace srv {
    class GetPrivateChannelRequest;
    class GetChannelRequest;
    class CreateChannelRequest;
    class UpdateChannelRequest;
    class DeleteChannelRequest;
    class ListUserChannelsRequest;
    class ListUserChannelsReponse;
    class CheckIsMemberChannelRequest;
    class ListUserChannelsReponse;
    class CheckIsMemberChannelRequest;
    class CheckIsMemberChannelReponse;
    class CheckIsMemberAndGetChannelRequest;
    class CheckIsMemberAndGetChannelReponse;
    class AddMembersChannelRequest;
    class RemoveMemberChannelRequest;
}

template <class T> class Repository {
public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  inline auto getChannel(const ::srv::GetChannelRequest *request) {
    return static_cast<T &>(*this).getChannelImpl(request);
  }

  inline auto
  createChannel(const ::srv::CreateChannelRequest *request) {
    return static_cast<T &>(*this).createChannelImpl(request);
  }

  inline auto
  updateChannel(const ::srv::UpdateChannelRequest *request) {
    return static_cast<T &>(*this).updateChannelImpl(request);
  }

  inline auto
  deleteChannel(const ::srv::DeleteChannelRequest *request) {
    return static_cast<T &>(*this).deleteChannelImpl(request);
  }

  inline auto
  listUserChannels(const ::srv::ListUserChannelsRequest *request) {
    return static_cast<T &>(*this).listUserChannelsImpl(request);
  }

  inline auto
  checkIsMemberChannel(const ::srv::CheckIsMemberChannelRequest *request) {
    return static_cast<T &>(*this).checkIsMemberChannelImpl(request);
  }

  inline auto checkIsMemberAndGetChannel(
      const ::srv::CheckIsMemberAndGetChannelRequest *request) {
    return static_cast<T &>(*this).checkIsMemberAndGetChannelImpl(request);
  }

  inline auto
  addMembersChannel(const ::srv::AddMembersChannelRequest *request) {
    return static_cast<T &>(*this).addMembersChannelImpl(request);
  }

  inline auto
  removeMemberChannel(const ::srv::RemoveMemberChannelRequest *request) {
    return static_cast<T &>(*this).removeMemberChannelImpl(request);
  }

  inline auto
  getPrivateChannel(const ::srv::GetPrivateChannelRequest *request) {
    return static_cast<T &>(*this).getPrivateChannelImpl(request);
  }
};
