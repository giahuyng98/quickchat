#pragma once

#include "money_service.pb.h"
#include <cstdint>
#include <string_view>
template <class T> class Repository {

public:
  template <class Config> inline void start(const Config &config) {
    static_cast<T &>(*this).startImpl(config);
  }

  auto inline createAccount(const uint64_t userId,
                            const std::string_view type) {
    return static_cast<T &>(*this).createAccountImpl(userId, type);
  }

  auto inline getBalance(const uint64_t userId) {
    return static_cast<T &>(*this).getBalanceImpl(userId);
  }

  auto inline deposit(const ::srv::DepositRequest *request) {
    return static_cast<T &>(*this).depositImpl(request);
  }

  auto inline withdraw(const ::srv::WithDrawRequest *request) {
    return static_cast<T &>(*this).withdrawImpl(request);
  }

  auto inline transfer(const ::srv::TransferRequest *request) {
    return static_cast<T &>(*this).transferImpl(request);
  }

  auto inline history(const ::srv::HistoryRequest *request) {
    return static_cast<T &>(*this).historyImpl(request);
  }
};
