#pragma once

#include "logger.hpp"
#include "message_repository_impl.hpp"
#include "message_service.grpc.pb.h"
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

template <class Repository>
class MessageServiceImpl final : public srv::MessageService::Service {

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
    logger::info("rpc message service listening on {}", address);
    server->Wait();
  }

private:
  ::grpc::Status GetMessages(::grpc::ServerContext *context,
                             const ::srv::GetMessagesRequest *request,
                             ::srv::GetMessagesReponse *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.getMessages(request);
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
  GetLastMessage(::grpc::ServerContext *context,
                 const ::srv::GetLastMessageRequest *request,
                 ::srv::GetLastMessageReponse *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.getLastMessage(request);
        return grpc::Status::OK;
      }
      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");

    } catch (const mysqlpp::BadQuery &ex) {
      logger::error("bad query: {}, {}", ex.what(), ex.errnum());
    } catch (const mysqlpp::BadConversion &ex) {
      logger::error("bad con: {}", ex.what());
    } catch (std::exception &ex) {
      logger::error(ex.what());
    }
    return grpc::Status::OK;
  }
};
