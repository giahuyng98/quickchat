#pragma once

#include "account.h"
#include "account.pb.h"
#include "gen_id_service_impl.h"
#include "history.pb.h"
#include "logger.hpp"
#include "money_service.pb.h"
#include "repository.hpp"
#include "request.h"
#include "transaction_history.h"
#include <algorithm>
#include <exception>
#include <google/protobuf/util/time_util.h>
#include <mysql++/datetime.h>
#include <mysql++/exceptions.h>
#include <mysql++/mysql++.h>
#include <mysql++/null.h>
#include <mysql++/query.h>
#include <mysql++/result.h>
#include <mysql++/row.h>
#include <mysql++/sql_types.h>
#include <mysql++/transaction.h>

namespace detail {

constexpr uint64_t DEFAULT_BALANCE = 0;
msg::Account toAccount(const model::Account &account) {
  msg::Account result;
  result.set_user_id(account.user_id);
  result.set_type(account.type);
  result.set_balance(account.balance);
  return result;
}

model::Account toAccount(const msg::Account &account) {
  return model::Account(account.user_id(), DEFAULT_BALANCE, account.type());
}

msg::History parseHistory(const mysqlpp::Row &row) {
  msg::History res;
  res.set_transaction_id(row["id"]);
  res.set_user_id(row["user_id"]);
  res.set_request_id(row["request_id"]);
  res.set_type(row["type"]);
  res.set_to_user(row["to"]);
  res.set_amount(row["amount"]);
  res.set_status(row["status"]);

  mysqlpp::sql_varchar_null message = row["message"];
  if (!message.is_null) {
    res.set_message(message.data);
  }
  std::string createAt;
  row["create_at"].to_string(createAt);

  google::protobuf::util::TimeUtil::FromString(createAt,
                                               res.mutable_create_at());

  return res;
};
} // namespace detail

class RepositoryImpl : public Repository<RepositoryImpl> {
  friend class Repository<RepositoryImpl>;

  mysqlpp::Connection conn;
  GenIdServiceImpl genIdService;

private:
  template <class Config> void startImpl(const Config &config) {
    model::Account::table("accounts");
    model::Request::table("requests");
    model::TransactionHistory::table("transactions_history");

    genIdService.instance().start(config);

    conn.connect(
        config["mysql"]["db"].template get<std::string>().c_str(),
        config["mysql"]["server"].template get<std::string>().c_str(),
        config["mysql"]["user"].template get<std::string>().c_str(),
        config["mysql"]["password"].template get<std::string>().c_str());
  }

  msg::Account getBalanceImpl(const uint64_t userId) {
    constexpr auto QUERY =
        "SELECT user_id, balance, type FROM accounts WHERE user_id = %0";
    mysqlpp::Query query = conn.query(QUERY);
    query.parse();

    msg::Account account;
    const auto queryResult = query.store(userId);

    if (queryResult.empty()) {
      throw mysqlpp::BadQuery("no user");
    }
    return detail::toAccount(queryResult[0]);
  }

  msg::Account createAccountImpl(const uint64_t userId,
                                 const std::string_view type) {
    mysqlpp::Query query = conn.query();
    msg::Account account;
    account.set_user_id(userId);
    account.set_balance(detail::DEFAULT_BALANCE);
    account.set_type(type.data(), type.size());

    query.insert(detail::toAccount(account));
    query.execute();

    if (query.affected_rows() != 0) {
      return account;
    }
    throw mysqlpp::BadQuery("can't create account");
  }

  msg::Account depositImpl(const ::srv::DepositRequest *request) {
    // create new request
    if (!createNewRequest(request->id(), request->user_id(), "deposit",
                          request->amount(), request->user_id(), "new",
                          mysqlpp::null)) {
      throw mysqlpp::BadQuery("can't execute insert request");
    }
    try {

      mysqlpp::Transaction tran(conn, mysqlpp::Transaction::repeatable_read,
                                mysqlpp::Transaction::this_transaction);
      // add balance
      if (!addBalance(request->user_id(), request->amount())) {
        throw mysqlpp::BadQuery("can't deposit");
      }

      // update request status
      if (!updateRequestStatus(request->id(), "success")) {
        throw mysqlpp::BadQuery("can't update request status");
      }

      const uint64_t transactionId = genIdService.instance().nextId();
      const mysqlpp::DateTime createAt = mysqlpp::NOW();
      const int64_t amount = request->amount();

      // add transaction history
      if (!createTransactionHistory(transactionId, request->user_id(), amount,
                                    request->id(), createAt)) {
        throw mysqlpp::BadQuery("can't create transaction history");
      }

      tran.commit();

      // update request status  error
      return getBalance(request->user_id());

    } catch (std::exception &ex) {
      updateRequestStatus(request->id(), "error");
      throw;
    }
  }

  msg::Account withdrawImpl(const ::srv::WithDrawRequest *request) {
    // create new request
    if (!createNewRequest(request->id(), request->user_id(), "withdraw",
                          request->amount(), request->user_id(), "new",
                          mysqlpp::null)) {
      throw mysqlpp::BadQuery("can't execute insert request");
    }
    try {
      mysqlpp::Transaction tran(conn, mysqlpp::Transaction::repeatable_read,
                                mysqlpp::Transaction::this_transaction);
      // subtract balance
      if (!subtractBalance(request->user_id(), request->amount())) {
        throw mysqlpp::BadQuery(
            "can't withdraw, not enought money or amount = 0");
      }

      // update request status
      if (!updateRequestStatus(request->id(), "success")) {
        throw mysqlpp::BadQuery("can't update request status");
      }

      const uint64_t transactionId = genIdService.instance().nextId();
      const mysqlpp::DateTime createAt = mysqlpp::NOW();
      // negative convert
      const int64_t amount = -request->amount();

      // add transaction history
      if (!createTransactionHistory(transactionId, request->user_id(), amount,
                                    request->id(), createAt)) {
        throw mysqlpp::BadQuery("can't create transaction history");
      }

      tran.commit();
      return getBalance(request->user_id());
    } catch (std::exception &ex) {
      updateRequestStatus(request->id(), "error");
      throw;
    }
  }

  msg::Account transferImpl(const ::srv::TransferRequest *request) {
    if (request->user_id() == request->to_user()) {
      throw mysqlpp::BadQuery("can't not transfer money to yourself");
    }
    // create new request
    mysqlpp::sql_varchar_null message = mysqlpp::null;
    if (!request->message().empty()) {
      message = request->message();
    }
    if (!createNewRequest(request->id(), request->user_id(), "transfer",
                          request->amount(), request->to_user(), "new",
                          message)) {
      throw mysqlpp::BadQuery("can't execute insert request");
    }
    try {
      mysqlpp::Transaction tran(conn, mysqlpp::Transaction::repeatable_read,
                                mysqlpp::Transaction::this_transaction);
      // subtract balance
      if (!subtractBalance(request->user_id(), request->amount())) {
        throw mysqlpp::BadQuery("not enought money or amount = 0");
      }

      // add balance
      if (!addBalance(request->to_user(), request->amount())) {
        throw mysqlpp::BadQuery("can't transfer");
      }

      // update request status
      if (!updateRequestStatus(request->id(), "success")) {
        throw mysqlpp::BadQuery("can't update request status");
      }

      const uint64_t transactionId = genIdService.instance().nextId();
      const mysqlpp::DateTime createAt = mysqlpp::NOW();
      // negative convert
      const int64_t add = request->amount();
      const int64_t sub = -request->amount();

      // add transaction history
      // subtract balance
      if (!createTransactionHistory(transactionId, request->user_id(), sub,
                                    request->id(), createAt)) {
        throw mysqlpp::BadQuery("can't create transaction history");
      }
      // add transaction history
      // add balance
      if (!createTransactionHistory(transactionId, request->to_user(), add,
                                    request->id(), createAt)) {
        throw mysqlpp::BadQuery("can't create transaction history");
      }

      tran.commit();
      return getBalance(request->user_id());
    } catch (std::exception &ex) {
      updateRequestStatus(request->id(), "error");
      throw;
    }
  }

  srv::HistoryReponse historyImpl(const ::srv::HistoryRequest *request) {
    constexpr auto queryString =
        "SELECT th.id, th.user_id, r.id AS request_id, r.type, r.to, "
        "th.amount, r.status, r.message, "
        "DATE_FORMAT(r.create_at,'%Y-%m-%dT%h:%i:%sZ') AS create_at FROM "
        "transactions_history th JOIN requests r ON th.request_id = r.id WHERE "
        "th.user_id=%0 LIMIT %1 OFFSET %2";

    mysqlpp::Query query = conn.query(queryString);
    query.parse();

    constexpr int defaultLimit = 100;

    const int limit = request->limit() == 0 ? defaultLimit : request->limit();
    const int offset = request->offset();

    const mysqlpp::StoreQueryResult queryResult =
        query.store(request->user_id(), limit, offset);

    srv::HistoryReponse reponse;

    std::for_each(std::begin(queryResult), std::end(queryResult),
                  [&](const mysqlpp::Row &row) {
                    reponse.mutable_histories()->Add(detail::parseHistory(row));
                  });
    return reponse;
  }

private:
  bool createNewRequest(const std::string_view id, const uint64_t userId,
                        const std::string_view type, const uint64_t amount,
                        const uint64_t to, const std::string_view status,
                        const mysqlpp::sql_varchar_null message) {
    model::Request request(id.data(), userId, type.data(), amount, to,
                           status.data(), message, mysqlpp::NOW());
    mysqlpp::Query query = conn.query();
    query.insert(request);
    return query.execute();
  }

  bool updateRequestStatus(const std::string_view id,
                           const std::string_view status = "success") {
    constexpr auto UPDATE_STATUS =
        "UPDATE requests SET status = %0q WHERE id = %1q";
    mysqlpp::Query query = conn.query(UPDATE_STATUS);
    query.parse();
    return query.execute(status.data(), id.data());
  }

  bool createTransactionHistory(const uint64_t id, const uint64_t userId,
                                const uint64_t amount,
                                const std::string_view requestId,
                                const mysqlpp::DateTime &createAt) {
    model::TransactionHistory transactionHistory(id, userId, amount,
                                                 requestId.data(), createAt);
    mysqlpp::Query query = conn.query();
    query.insert(transactionHistory);
    return query.execute();
  }

  bool addBalance(const uint64_t userId, const uint64_t amount) {
    constexpr auto DEPOSIT =
        "UPDATE accounts SET balance = balance + %0 WHERE user_id = %1";
    mysqlpp::Query query = conn.query(DEPOSIT);
    query.parse();
    query.execute(amount, userId);
    return query.affected_rows() == 1;
  }

  bool subtractBalance(const uint64_t userId, const uint64_t amount) {
    constexpr auto WITHDRAW = "UPDATE accounts SET balance = balance - %0 "
                              "WHERE user_id = %1 AND balance - %0 >= 0";
    mysqlpp::Query query = conn.query(WITHDRAW);
    query.parse();
    query.execute(amount, userId);
    // TODO: ??
    return query.affected_rows() == 1;
  }
};
