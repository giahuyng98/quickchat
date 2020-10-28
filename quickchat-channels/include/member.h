#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

//#if !defined(EXPAND_MY_SSQLS_STATICS)
//#define MYSQLPP_SSQLS_NO_STATICS
//#endif
namespace model {
//    constexpr std::string_view USER_FIELDS[] = {"username", "email", "fullname", "password"};

    sql_create_complete_5(Member, 1, 5,
                          mysqlpp::sql_bigint, channel_id, "channel_id",
                          mysqlpp::sql_bigint, user_id, "user_id",
                          mysqlpp::sql_enum, role, "role",
                          mysqlpp::sql_datetime, createAt, "create_at",
                          mysqlpp::sql_datetime_null, deleteAt, "delete_at")
}

