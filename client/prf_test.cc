#include "gtest/gtest.h"

#include "openssl/bn.h"

#include "audit/client/prf.h"

using namespace audit;

TEST(HMACPRF, Same) {
  HMACPRF prf_hello{HMACPRF::KeyType{}};
  HMACPRF prf_hello2{HMACPRF::KeyType{}};

  auto a = prf_hello.Encode(10);
  auto b = prf_hello2.Encode(10);

  EXPECT_EQ(0, BN_cmp(a.get(), b.get()));
}

TEST(HMACPRF, Different) {
  HMACPRF prf_hello{HMACPRF::KeyType{}};

  HMACPRF::KeyType key;
  key[0] = 'a';
  key[1] = 'b';
  HMACPRF prf_hello2{key};

  auto a = prf_hello.Encode(10);
  auto b = prf_hello2.Encode(10);

  EXPECT_NE(0, BN_cmp(a.get(), b.get()));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
