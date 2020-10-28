#include "session_mgr_impl.hpp"
#include <gtest/gtest.h>

TEST(SessionMgr, addRemove) {

  SessionMgrImpl<int, std::string> sessionMgr;

  {
    sessionMgr.store(1, "str1");
    sessionMgr.store(1, "str2");
    sessionMgr.store(1, "str3");
    sessionMgr.store(1, "str4");

    auto lhs = sessionMgr.getList(1);

    std::vector<std::string> rhs = {"str1", "str2", "str3", "str4"};
    EXPECT_EQ(lhs, rhs);
  }

  {
    bool deleted = sessionMgr.remove(1, "str1");
    EXPECT_TRUE(deleted);

    auto lhs = sessionMgr.getList(1);
    std::vector<std::string> rhs = {"str2", "str3", "str4"};
    EXPECT_EQ(lhs, rhs);
  }

  {
    int k = 5;
    std::string v = "42";
    sessionMgr.store(k, v);
  }
}

TEST(SessionMgr, getEmpty) {
  SessionMgrImpl<int, std::string> sessionMgr;
  std::vector<std::string> rhs = {};
  EXPECT_EQ(sessionMgr.getList(1), rhs);
}
