#include <gtest/gtest.h>

//#include "uWebSocket.hpp"


//TEST(WebSocket, test_singleton) {
  //auto &ins1 = WebSocket<uWebSocket>::instance();
  //auto &ins2 = WebSocket<uWebSocket>::instance();

  //ASSERT_EQ(&ins1, &ins2);
  //EXPECT_EQ(&ins1, &ins2);
//}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
