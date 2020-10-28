#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace model {
  sql_create_complete_4(Friend, 1, 4, 
                        mysqlpp::sql_bigint, uid1, "uid1",
                        mysqlpp::sql_bigint, uid2, "uid2",
                        mysqlpp::sql_enum, status, "status",
                        mysqlpp::sql_datetime, createAt, "create_at"
                        )
}
