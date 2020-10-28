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

    sql_create_complete_6(Message, 1, 6,
                          mysqlpp::sql_bigint, channelId, "channel_id",
                          mysqlpp::sql_bigint, messageId, "message_id",
                          mysqlpp::sql_bigint, userId, "user_id",
                          mysqlpp::sql_blob, content, "content",
                          mysqlpp::sql_datetime, createAt, "create_at",
                          mysqlpp::sql_datetime_null, deleteAt, "delete_at")
}

