#pragma once

#include "account.pb.h"
#include "logger.hpp"
#include "money_service.grpc.pb.h"
#include "money_service.hpp"
#include "money_service.pb.h"
#include <grpcpp/grpcpp.h>

class MoneyServiceImpl : public MoneyService<MoneyServiceImpl> {
  friend class MoneyService<MoneyServiceImpl>;

  std::unique_ptr<srv::MoneyService::Stub> stub;
  std::string apikey;

private:
  template <class Config> void startImpl(const Config &config) {
    logger::info("moneyservice is started");

    apikey = config["money-service"]["apikey"].template get<std::string>();
    const std::string moneyServiceURI =
        config["money-service"]["uri"].template get<std::string>();

    stub = srv::MoneyService::NewStub(grpc::CreateChannel(
        moneyServiceURI, grpc::InsecureChannelCredentials()));
  }

  std::pair<grpc::Status, msg::Account> getAccountImpl(const uint64_t userId) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);

    srv::GetBalanceRequest req;
    req.set_user_id(userId);
    msg::Account res;
    auto status = stub->GetBalance(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, msg::Account>
  createAccountImpl(const srv::CreateAccountRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    msg::Account res;
    auto status = stub->CreateAccount(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, msg::Account>
  depositImpl(const srv::DepositRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    msg::Account res;
    auto status = stub->Deposit(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, msg::Account>
  withDrawImpl(const srv::WithDrawRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    msg::Account res;
    auto status = stub->WithDraw(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, msg::Account>
  transferImpl(const srv::TransferRequest &req) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    msg::Account res;
    auto status = stub->Transfer(&context, req, &res);

    return std::make_pair(status, res);
  }

  std::pair<grpc::Status, srv::HistoryReponse>
  historyImpl(const uint64_t userId, const uint64_t limit,
              const uint64_t offset) {
    grpc::ClientContext context;
    context.AddMetadata("apikey", apikey);
    srv::HistoryRequest req;
    req.set_user_id(userId);
    req.set_limit(limit);
    req.set_offset(offset);

    srv::HistoryReponse res;
    auto status = stub->History(&context, req, &res);
    //logger::info("req: {}", req.DebugString());
    //logger::info("res: {}", res.DebugString());
    return std::make_pair(status, res);
  }
};
