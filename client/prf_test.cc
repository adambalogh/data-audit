#include "gtest/gtest.h"

#include "openssl/bn.h"

#include "audit/client/prf.h"

using namespace audit;

TEST(HMACPRF, AutoGenerateKey) {
  HMACPRF prf_1;
  HMACPRF prf_2;

  auto a = prf_1.Encode(10);
  auto b = prf_2.Encode(10);

  EXPECT_NE(0, BN_cmp(a.get(), b.get()));
}

TEST(HMACPRF, SetKey) {
  HMACPRF prf_1;
  HMACPRF prf_2{prf_1.Key()};

  auto a = prf_1.Encode(10);
  auto b = prf_2.Encode(10);

  EXPECT_EQ(0, BN_cmp(a.get(), b.get()));
}

TEST(HMACPRF, Same) {
  HMACPRF prf_hello;

  auto a = prf_hello.Encode(10);
  auto b = prf_hello.Encode(10);

  EXPECT_EQ(0, BN_cmp(a.get(), b.get()));
}

TEST(HMACPRF, Different) {
  HMACPRF prf_hello;
  HMACPRF prf_hello2;

  auto a = prf_hello.Encode(10);
  auto b = prf_hello2.Encode(10);

  EXPECT_NE(0, BN_cmp(a.get(), b.get()));
}

TEST(HMACPRF, Key) {
  HMACPRF prf;
  HMACPRF::KeyType sample_key;

  EXPECT_EQ(sample_key.size(), prf.Key().size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
