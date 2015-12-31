#include "gtest/gtest.h"

#include <fstream>
#include <sstream>

#include "openssl/bn.h"

#include "audit/test_util.h"
#include "audit/client/upload/file.h"

using namespace audit;

TEST(File, ThrowsExceptionWhenInvalidFile) {
  std::ifstream s{""};
  EXPECT_THROW(File(s, ""), std::runtime_error);
}

TEST(TaggingParameters, BlockSize) {
  TaggingParameters params{10, 4};

  EXPECT_EQ(40, params.block_size());
}

TEST(FileContext, AlphaInvalidSize) {
  std::stringstream s;
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, ""};
  TaggingParameters parameters{2, 1};
  EXPECT_THROW(FileContext(file, parameters, make_BN_vector({1}), std::move(p),
                           std::unique_ptr<PRF>{new DummyPRF}),
               std::length_error);
}

TEST(FileContext, AlphaSize) {
  std::stringstream s;
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, ""};
  TaggingParameters parameters{2, 1};
  EXPECT_NO_THROW(FileContext(file, parameters, make_BN_vector({1, 1}),
                              std::move(p),
                              std::unique_ptr<PRF>{new DummyPRF}));
}

TEST(FileContext, NumBlocks) {
  // Length is 10
  std::stringstream s{"aaaaaaaaaa"};
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, ""};
  TaggingParameters parameters{2, 1};
  FileContext context{file, parameters, make_BN_vector({1, 1}), std::move(p),
                      std::unique_ptr<PRF>{new DummyPRF}};

  EXPECT_EQ(5, context.num_blocks());
}

TEST(FileContext, NumBlocksLastBlockNotFull) {
  // Length is 9
  std::stringstream s{"aaaaaaaaa"};
  BN_ptr p{BN_new(), ::BN_free};
  File file{s, ""};
  TaggingParameters parameters{2, 1};
  FileContext context{file, parameters, make_BN_vector({1, 1}), std::move(p),
                      std::unique_ptr<PRF>{new DummyPRF}};
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
