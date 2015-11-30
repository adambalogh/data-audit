#include "gtest/gtest.h"

#include <sstream>

#include "openssl/bn.h"

#include "audit/test_util.h"
#include "audit/client/file_tag.h"

using namespace audit;

TEST(FileTag, NumBlocks) {
  std::stringstream file{"aaaaaaaaa"};
  ConstantNumberGenerator<1> g;
  BN_ptr p{BN_new(), ::BN_free};
  FileTag tag{file, 2, 1, std::move(p), &g};

  EXPECT_EQ(5, tag.num_blocks());
}

TEST(FileTag, Alphas) {
  std::stringstream file{"aaaaaaaaa"};
  DummyNumberGenerator g{{10, 5}};
  BN_ptr p{BN_new(), ::BN_free};
  FileTag tag{file, 2, 1, std::move(p), &g};

  EXPECT_EQ(2, tag.alphas().size());
  EXPECT_EQ(10, tag.alphas().at(0));
  EXPECT_EQ(5, tag.alphas().at(1));
}

TEST(FileTag, BlockSize) {
  std::stringstream file;
  ConstantNumberGenerator<1> g;
  BN_ptr p{BN_new(), ::BN_free};
  FileTag tag{file, 2, 5, std::move(p), &g};

  EXPECT_EQ(10, tag.block_size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
