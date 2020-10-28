#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace model {

sql_create_complete_5(TransactionHistory, 1, 5,
                      mysqlpp::sql_bigint, id, "id",
                      mysqlpp::sql_bigint, user_id, "user_id",
                      mysqlpp::sql_bigint, amount, "amount",
                      mysqlpp::sql_char, request_id, "request_id",
                      mysqlpp::sql_datetime, createAt, "create_at"
                      );
}
