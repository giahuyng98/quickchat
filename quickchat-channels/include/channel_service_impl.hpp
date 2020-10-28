#pragma once
#include "channel_service.pb.h"
#include "logger.hpp"
#include "proto/channel_service.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <mysql++/exceptions.h>

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
class ChannelServiceImpl final : public srv::ChannelService::Service {
  std::string apikey;
  Repository repo;

public:
  template <class Config> void start(const Config &config);

private:
  bool authen(std::string_view key);

  ::grpc::Status GetChannel(::grpc::ServerContext *context,
                            const ::srv::GetChannelRequest *request,
                            ::msg::Channel *response) override;
  ::grpc::Status CreateChannel(::grpc::ServerContext *context,
                               const ::srv::CreateChannelRequest *request,
                               ::msg::Channel *response) override;

  ::grpc::Status UpdateChannel(::grpc::ServerContext *context,
                               const ::srv::UpdateChannelRequest *request,
                               ::msg::Channel *response) override;

  ::grpc::Status DeleteChannel(::grpc::ServerContext *context,
                               const ::srv::DeleteChannelRequest *request,
                               ::google::protobuf::Empty *response) override;
  // custom
  ::grpc::Status
  ListUserChannels(::grpc::ServerContext *context,
                   const ::srv::ListUserChannelsRequest *request,
                   ::srv::ListUserChannelsReponse *response) override;
  ::grpc::Status
  CheckIsMemberChannel(::grpc::ServerContext *context,
                       const ::srv::CheckIsMemberChannelRequest *request,
                       ::srv::CheckIsMemberChannelReponse *response) override;

  ::grpc::Status CheckIsMemberAndGetChannel(
      ::grpc::ServerContext *context,
      const ::srv::CheckIsMemberAndGetChannelRequest *request,
      ::srv::CheckIsMemberAndGetChannelReponse *response) override;

  ::grpc::Status
  AddMembersChannel(::grpc::ServerContext *context,
                    const ::srv::AddMembersChannelRequest *request,
                    ::msg::Channel *response) override;
  ::grpc::Status
  RemoveMemberChannel(::grpc::ServerContext *context,
                      const ::srv::RemoveMemberChannelRequest *request,
                      ::msg::Channel *response) override;

  ::grpc::Status
  GetPrivateChannel(::grpc::ServerContext *context,
                    const ::srv::GetPrivateChannelRequest *request,
                    ::msg::Channel *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.getPrivateChannel(request);
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
};

template <class Repository>
template <class Config>
void ChannelServiceImpl<Repository>::start(const Config &config) {
  repo.start(config);

  apikey = config["rpc"]["apikey"].template get<std::string>();

  grpc::EnableDefaultHealthCheckService(true);
  const std::string address = config["rpc"]["uri"].template get<std::string>();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());

  builder.RegisterService(this);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  logger::info("rpc channel service listening on {}", address);
  server->Wait();
}

template <class Repository>
bool ChannelServiceImpl<Repository>::authen(const std::string_view key) {
  return this->apikey == key;
}

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::GetChannel(
    ::grpc::ServerContext *context, const ::srv::GetChannelRequest *request,
    ::msg::Channel *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.getChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::CreateChannel(
    ::grpc::ServerContext *context, const ::srv::CreateChannelRequest *request,
    ::msg::Channel *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.createChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::UpdateChannel(
    ::grpc::ServerContext *context, const ::srv::UpdateChannelRequest *request,
    ::msg::Channel *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      //*response = repo.
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::CheckIsMemberChannel(
    ::grpc::ServerContext *context,
    const ::srv::CheckIsMemberChannelRequest *request,
    ::srv::CheckIsMemberChannelReponse *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.checkIsMemberChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::ListUserChannels(
    ::grpc::ServerContext *context,
    const ::srv::ListUserChannelsRequest *request,
    ::srv::ListUserChannelsReponse *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.listUserChannels(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::CheckIsMemberAndGetChannel(
    ::grpc::ServerContext *context,
    const ::srv::CheckIsMemberAndGetChannelRequest *request,
    ::srv::CheckIsMemberAndGetChannelReponse *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.checkIsMemberAndGetChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::AddMembersChannel(
    ::grpc::ServerContext *context,
    const ::srv::AddMembersChannelRequest *request, ::msg::Channel *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.addMembersChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::RemoveMemberChannel(
    ::grpc::ServerContext *context,
    const ::srv::RemoveMemberChannelRequest *request,
    ::msg::Channel *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.removeMemberChannel(request);
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

template <class Repository>
::grpc::Status ChannelServiceImpl<Repository>::DeleteChannel(
    ::grpc::ServerContext *context, const ::srv::DeleteChannelRequest *request,
    ::google::protobuf::Empty *response) {
  try {
    if (authen(detail::getApiKey(context))) {
      *response = repo.deleteChannel(request);
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
