#include "gtest/gtest.h"

#include <sstream>

#include "openssl/bn.h"

#include "audit/test_util.h"
#include "audit/client/file.h"

using namespace audit;

TEST(File, NumBlocks) {
  std::stringstream s{"aaaaaaaaaa"};
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, "", 2, 1, make_BN_vector({1, 1}), std::move(p)};

  EXPECT_EQ(5, tag.num_blocks());
}

TEST(File, NumBlocksLastBlockNotFull) {
  std::stringstream s{"aaaaaaaaa"};
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, "", 2, 1, make_BN_vector({1, 1}), std::move(p)};

  EXPECT_EQ(5, tag.num_blocks());
}

TEST(File, Alphas) {
  std::stringstream s{"aaaaaaaaa"};
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, "", 2, 1, make_BN_vector({10, 5}), std::move(p)};

  EXPECT_EQ(2, tag.alphas().size());
  EXPECT_EQ(10, tag.alphas().at(0));
  EXPECT_EQ(5, tag.alphas().at(1));
}

TEST(File, BlockSize) {
  std::stringstream s;
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, "", 2, 5, make_BN_vector({1, 1}), std::move(p)};

  EXPECT_EQ(10, tag.block_size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
