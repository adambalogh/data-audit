#include "gtest/gtest.h"

#include "util.h"

#include "cryptopp/integer.h"

using namespace audit;

TEST(Util, CryptoEncodeDecode) {
  CryptoPP::Integer original{14323454};

  std::string encoded;
  CryptoIntegerToString(original, &encoded);
  CryptoPP::Integer decoded;
  StringToCryptoInteger(encoded, &decoded);

  EXPECT_EQ(decoded, original);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
