#pragma once

#include "logger.hpp"
#include "money_service.grpc.pb.h"

#include <exception>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <memory>

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
class MoneyServiceImpl final : public srv::MoneyService::Service {

  Repository repo;
  std::string apikey;

public:
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
    logger::info("rpc money service listening on {}", address);
    server->Wait();
  }

private:
  bool authen(std::string_view key) { return this->apikey == key; }

  ::grpc::Status GetBalance(::grpc::ServerContext *context,
                            const ::srv::GetBalanceRequest *request,
                            ::msg::Account *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.getBalance(request->user_id());
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status CreateAccount(::grpc::ServerContext *context,
                               const ::srv::CreateAccountRequest *request,
                               ::msg::Account *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.createAccount(request->user_id(), request->type());
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status Deposit(::grpc::ServerContext *context,
                         const ::srv::DepositRequest *request,
                         ::msg::Account *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.deposit(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status WithDraw(::grpc::ServerContext *context,
                          const ::srv::WithDrawRequest *request,
                          ::msg::Account *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.withdraw(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status Transfer(::grpc::ServerContext *context,
                          const ::srv::TransferRequest *request,
                          ::msg::Account *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.transfer(request);
        return grpc::Status::OK;
      }

      return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                          "permission denied");
    } catch (std::exception &ex) {
      logger::error(ex.what());
      return grpc::Status(grpc::StatusCode::UNKNOWN, ex.what());
    }
  }

  ::grpc::Status History(::grpc::ServerContext *context,
                         const ::srv::HistoryRequest *request,
                         ::srv::HistoryReponse *response) override {
    try {
      if (authen(detail::getApiKey(context))) {
        *response = repo.history(request);
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
