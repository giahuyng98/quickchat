#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace model {

sql_create_complete_3(Account, 1, 3, 
                      mysqlpp::sql_bigint, user_id, "user_id",
                      mysqlpp::sql_bigint, balance, "balance",
                      mysqlpp::sql_enum, type, "type"
                      );
}
