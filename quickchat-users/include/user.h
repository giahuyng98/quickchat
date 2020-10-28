//
// Created by giahuy on 9/5/20.
//

#ifndef CCHAT_USER_H
#define CCHAT_USER_H

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

#if !defined(EXPAND_MY_SSQLS_STATICS)
#define MYSQLPP_SSQLS_NO_STATICS
#endif
namespace model {
//    constexpr std::string_view USER_FIELDS[] = {"username", "email",
//    "fullname", "password"};
sql_create_5(User, 1, 5, 
    mysqlpp::sql_bigint, id, 
    mysqlpp::sql_varchar, username, 
    mysqlpp::sql_varchar, email, 
    mysqlpp::sql_varchar, fullname, 
    mysqlpp::sql_varchar, password)
} // namespace model

#endif // CCHAT_USER_H
