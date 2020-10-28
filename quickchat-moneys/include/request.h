#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace model {

sql_create_complete_8(Request, 1, 8,
                      mysqlpp::sql_char, id, "id",
                      mysqlpp::sql_bigint, user_id, "user_id",
                      mysqlpp::sql_enum, type, "type",
                      mysqlpp::sql_bigint, amount, "amount",
                      mysqlpp::sql_bigint_null, to, "to",
                      mysqlpp::sql_enum, status, "status",
                      mysqlpp::sql_varchar_null, message, "message",
                      mysqlpp::sql_datetime, createAt, "create_at"
                      );
}
