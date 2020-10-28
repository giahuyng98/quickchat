#pragma once

#include "money_service.grpc.pb.h"
#include "money_service.pb.h"
#include <cstdint>

template <class T> class MoneyService {
public:
  template <class Config> inline void start(const Config &config) {
    return static_cast<T &>(*this).startImpl(config);
  }

  auto inline getAccount(const uint64_t userId) {
    return static_cast<T &>(*this).getAccountImpl(userId);
  }

  auto inline createAccount(const srv::CreateAccountRequest &req) {
    return static_cast<T &>(*this).createAccountImpl(req);
  }

  auto inline deposit(const srv::DepositRequest &req) {
    return static_cast<T &>(*this).depositImpl(req);
  }

  auto inline withDraw(const srv::WithDrawRequest &req) {
    return static_cast<T &>(*this).withDrawImpl(req);
  }

  auto inline transfer(const srv::TransferRequest &req) {
    return static_cast<T &>(*this).transferImpl(req);
  }

  auto inline history(const uint64_t userId, const int limit,
                      const int offset) {
    return static_cast<T &>(*this).historyImpl(userId, limit, offset);
  }
};
