#include "gtest/gtest.h"

#include <fstream>
#include <sstream>

#include "openssl/bn.h"

#include "audit/test_util.h"
#include "audit/client/upload/file.h"

using namespace audit::upload;

TEST(File, ThrowsExceptionWhenInvalidFile) {
  // invalid file
  auto empty = std::make_unique<std::ifstream>("");

  EXPECT_THROW(File(std::move(empty), ""), std::runtime_error);
}

TEST(TaggingParameters, BlockSize) {
  TaggingParameters params{10, 4};

  EXPECT_EQ(40, params.block_size());
}

TEST(FileContext, AlphaInvalidSize) {
  BN_ptr p{BN_new(), ::BN_free};
  File file{EmptyFile(), ""};
  TaggingParameters parameters{2, 1};
  EXPECT_THROW(FileContext(file, parameters, make_BN_vector({1}), std::move(p),
                           std::unique_ptr<PRF>{new DummyPRF}),
               std::length_error);
}

TEST(FileContext, AlphaSize) {
  BN_ptr p{BN_new(), ::BN_free};
  File file{EmptyFile(), ""};
  TaggingParameters parameters{2, 1};
  EXPECT_NO_THROW(FileContext(file, parameters, make_BN_vector({1, 1}),
                              std::move(p),
                              std::unique_ptr<PRF>{new DummyPRF}));
}

TEST(FileContext, NumBlocks) {
  // Length is 10
  auto s = MakeFile("aaaaaaaaaa");
  BN_ptr p{BN_new(), ::BN_free};
  File file{std::move(s), ""};
  TaggingParameters parameters{2, 1};
  FileContext context{file, parameters, make_BN_vector({1, 1}), std::move(p),
                      std::unique_ptr<PRF>{new DummyPRF}};

  EXPECT_EQ(5, context.num_blocks());
}

TEST(FileContext, NumBlocksLastBlockNotFull) {
  // Length is 9
  auto s = MakeFile("aaaaaaaaa");
  BN_ptr p{BN_new(), ::BN_free};
  File file{std::move(s), ""};
  TaggingParameters parameters{2, 1};
  FileContext context{file, parameters, make_BN_vector({1, 1}), std::move(p),
                      std::unique_ptr<PRF>{new DummyPRF}};
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
