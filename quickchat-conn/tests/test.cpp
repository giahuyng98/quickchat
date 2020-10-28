#include <gtest/gtest.h>

constexpr int fac(int num) {
  return num == 0 ? 1 : num * fac(num - 1);
}

TEST(Test01, Test) {
  EXPECT_EQ(1, 2);
}

TEST(Test01, Fac) {
  EXPECT_EQ(fac(5), 120);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
