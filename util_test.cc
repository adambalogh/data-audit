#include "gtest/gtest.h"

#include "util.h"

#include "openssl/bn.h"

using namespace audit;

TEST(Util, CryptoEncodeDecode) {
  BN_ptr original{BN_new(), ::BN_free};
  BN_add_word(original.get(), 1000432);

  auto encoded = BignumToString(*original);
  auto decoded = StringToBignum(encoded);

  EXPECT_EQ(0, BN_cmp(decoded.get(), original.get()));

  BN_free(original);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
