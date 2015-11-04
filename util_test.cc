#include "gtest/gtest.h"

#include "util.h"

#include "openssl/bn.h"

using namespace audit;

TEST(Util, CryptoEncodeDecode) {
  BIGNUM* original = BN_new();
  BN_add_word(original, 1000432);

  auto encoded = BignumToString(*original);
  auto decoded = StringToBignum(encoded);

  EXPECT_EQ(0, BN_cmp(decoded, original));

  BN_free(original);
  BN_free(decoded);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
