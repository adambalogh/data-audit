#include "gtest/gtest.h"

#include <sstream>

#include "openssl/bn.h"

#include "audit/file_tag.h"

using namespace audit;

TEST(FileTag, NumBlocks) {
  std::stringstream file{"aaaaaaaaa"};
  ConstantNumberGenerator<1> g;
  BN_ptr p{BN_new(), ::BN_free};
  FileTag tag{file, 2, 1, std::move(p), &g};

  EXPECT_EQ(5, tag.num_blocks());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
