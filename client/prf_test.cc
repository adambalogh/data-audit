#include "gtest/gtest.h"

#include "openssl/bn.h"

#include "audit/client/prf.h"

using namespace audit;

TEST(SiphashPRF, Same) {
  SiphashPRF prf_hello{"hello"};
  SiphashPRF prf_hello2{"hello"};

  auto a = prf_hello.Encode(10);
  auto b = prf_hello.Encode(10);

  EXPECT_EQ(0, BN_cmp(a.get(), b.get()));

  auto c = prf_hello2.Encode(10);
  EXPECT_EQ(0, BN_cmp(a.get(), c.get()));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}