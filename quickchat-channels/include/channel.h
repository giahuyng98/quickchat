#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

//#if !defined(EXPAND_MY_SSQLS_STATICS)
//#define MYSQLPP_SSQLS_NO_STATICS
//#endif
namespace model {
//    constexpr std::string_view USER_FIELDS[] = {"username", "email", "fullname", "password"};

    sql_create_complete_6(Channel, 1, 6,
                          mysqlpp::sql_bigint, id, "id",
                          mysqlpp::sql_varchar_null, name, "name",
                          mysqlpp::sql_enum, type, "type",
                          mysqlpp::sql_blob_null, token, "token",
                          mysqlpp::sql_datetime, createAt, "create_at",
                          mysqlpp::sql_datetime_null, deleteAt, "delete_at")
}

