//
// Created by giahuy on 9/8/20.
//
#pragma once
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

//#if !defined(EXPAND_MY_SSQLS_STATICS)
//#define MYSQLPP_SSQLS_NO_STATICS
//#endif
namespace model {
//    constexpr std::string_view USER_FIELDS[] = {"username", "email", "fullname", "password"};

    sql_create_complete_4(Seen, 1, 4,
                          mysqlpp::sql_bigint, channelId, "channel_id",
                          mysqlpp::sql_bigint, messageId, "message_id",
                          mysqlpp::sql_bigint, userId, "user_id",
                          mysqlpp::sql_datetime, createAt, "create_at"
                          )
}

