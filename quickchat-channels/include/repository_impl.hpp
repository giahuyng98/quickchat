#pragma once

#include "channel_service.pb.h"
#define MYSQLPP_SSQLS_NO_STATICS
#include "channel.h"
#include "member.h"

#include "channel.pb.h"
#include "gen_id_service_impl.h"
#include "repository.hpp"
#include <google/protobuf/empty.pb.h>
#include <mysql++/mysql++.h>

class RepositoryImpl : public Repository<RepositoryImpl> {
public:
  friend class Repository<RepositoryImpl>;

private:
  template <class Config> void startImpl(const Config &config);

  msg::Channel getChannelImpl(const ::srv::GetChannelRequest *request);

  msg::Channel createChannelImpl(const ::srv::CreateChannelRequest *request);

  msg::Channel updateChannelImpl(const ::srv::UpdateChannelRequest *request);

  google::protobuf::Empty
  deleteChannelImpl(const ::srv::DeleteChannelRequest *request);

  ::srv::ListUserChannelsReponse
  listUserChannelsImpl(const ::srv::ListUserChannelsRequest *request);

  ::srv::CheckIsMemberChannelReponse
  checkIsMemberChannelImpl(const ::srv::CheckIsMemberChannelRequest *request);

  ::srv::CheckIsMemberAndGetChannelReponse checkIsMemberAndGetChannelImpl(
      const ::srv::CheckIsMemberAndGetChannelRequest *request);

  msg::Channel
  addMembersChannelImpl(const ::srv::AddMembersChannelRequest *request);

  msg::Channel
  removeMemberChannelImpl(const ::srv::RemoveMemberChannelRequest *request);

  msg::Channel
    getPrivateChannelImpl(const ::srv::GetPrivateChannelRequest *request);

private:
  msg::Channel createPrivateChannel(const ::srv::CreateChannelRequest *request);
  msg::Channel createGroupChannel(const ::srv::CreateChannelRequest *request);

private:
  mysqlpp::Connection conn;
};

template <class Config> void RepositoryImpl::startImpl(const Config &config) {
  model::Channel::table("channels");
  model::Member::table("members");
  GenIdServiceImpl::instance().start(config);

  conn.connect(config["mysql"]["db"].template get<std::string>().c_str(),
               config["mysql"]["server"].template get<std::string>().c_str(),
               config["mysql"]["user"].template get<std::string>().c_str(),
               config["mysql"]["password"].template get<std::string>().c_str());
}
