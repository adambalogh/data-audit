#include "gtest/gtest.h"

#include "prf.h"

using namespace audit;

TEST(SiphashPRF, Same) {
  SiphashPRF prf_hello{"hello"};
  SiphashPRF prf_hello2{"hello"};

  EXPECT_EQ(prf_hello.Encode(10), prf_hello.Encode(10));
  EXPECT_EQ(prf_hello.Encode(10), prf_hello2.Encode(10));

  std::cout << prf_hello.Encode(11);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
