#include "gtest/gtest.h"
#include "block_tagger.h"

#include <sstream>
#include <string>
#include <vector>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"
#include "audit/test_util.h"
#include "audit/util.h"

using namespace audit;

void ExpectProtosEqual(std::vector<proto::BlockTag> &expected,
                       std::vector<proto::BlockTag> &actual) {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected.at(i).DebugString(), actual.at(i).DebugString());
  }
}

FileTag BasicFileTag(std::istream &file) {
  BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 32452867);
  ConstantNumberGenerator<1> gen;
  return std::move(FileTag{file, 1, 1, std::move(p), &gen});
}

BlockTagger BasicBlockTagger(std::istream &file) {
  return std::move(BlockTagger{BasicFileTag(file),
                               std::move(std::unique_ptr<PRF>(new DummyPRF))});
}

TEST(BlockTagger, EmptyFile) {
  std::stringstream s;
  auto t = BasicBlockTagger(s);
  EXPECT_EQ(false, t.HasNext());
}

// TEST_F(BlockTaggerTest, NotEmptyFile) {
//  std::stringstream s{"a"};
//  auto t = GetBlockTagger(s);
//  EXPECT_EQ(true, t.HasNext());
//}

// TEST_F(BlockTaggerTest, SingleLetter) {
//  std::stringstream s{"a"};

//  file_tag.alphas.emplace_back(BN_new_ptr(10));
//  file_tag.num_sectors = 1;
//  file_tag.sector_size = 1;
//  auto t = GetBlockTagger(s);

//  auto tag = t.GetNext();
//  EXPECT_EQ(0, tag.index());
//  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
//}

// TEST_F(BlockTaggerTest, LargerSectorSize) {
//  std::stringstream s{"a"};

//  file_tag.alphas.emplace_back(BN_new_ptr(10));
//  file_tag.num_sectors = 1;
//  file_tag.sector_size = 10;
//  auto t = GetBlockTagger(s);

//  auto tag = t.GetNext();
//  EXPECT_EQ(0, tag.index());
//  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
//}

// TEST_F(BlockTaggerTest, LargerSectorNumber) {
//  std::stringstream s{"a"};

//  file_tag.alphas.emplace_back(BN_new_ptr(10));
//  file_tag.num_sectors = 10;
//  file_tag.sector_size = 1;
//  auto t = GetBlockTagger(s);

//  auto tag = t.GetNext();
//  EXPECT_EQ(0, tag.index());
//  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
//}

// TEST_F(BlockTaggerTest, LargeSectorSizeAndNumber) {
//  std::stringstream s{"a"};

//  file_tag.alphas.emplace_back(BN_new_ptr(10));
//  file_tag.num_sectors = 10;
//  file_tag.sector_size = 10;
//  auto t = GetBlockTagger(s);

//  auto tag = t.GetNext();
//  EXPECT_EQ(0, tag.index());
//  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
//}

// TEST_F(BlockTaggerTest, BecomesInvalid) {
//  std::stringstream s{"abc"};

//  file_tag.alphas.emplace_back(BN_new_ptr(10));
//  file_tag.num_sectors = 1;
//  auto t = GetBlockTagger(s);

//  t.GetNext();
//  EXPECT_EQ(false, t.HasNext());
//}

// TEST_F(BlockTaggerTest, Modulo) {
//  std::stringstream s{"a"};

//  file_tag.alphas.emplace_back(BN_new_ptr(100));
//  file_tag.p = BN_new_ptr(11);
//  auto t = GetBlockTagger(s);

//  int expected = (100 * static_cast<int>('a')) % 11;

//  auto tag = t.GetNext();
//  EXPECT_EQ(expected, StringToBignum(tag.sigma()));
//}

// TEST_F(BlockTaggerTest, NumBlocks) {
//  std::stringstream s{"aaaaa"};

//  file_tag.num_blocks = 100;
//  file_tag.alphas.emplace_back(BN_new_ptr(1));
//  file_tag.alphas.emplace_back(BN_new_ptr(1));
//  file_tag.num_sectors = 2;
//  file_tag.sector_size = 1;
//  auto t = GetBlockTagger(s);

//  while (t.HasNext()) {
//    t.GetNext();
//  }

//  EXPECT_EQ(3, file_tag.num_blocks);
//}

//// TODO clean up this
// std::string Result(BN_ptr a, BN_ptr b, unsigned int add) {
//  BN_add(a.get(), a.get(), b.get());
//  auto add_bn = BN_new_ptr(add);
//  BN_add(a.get(), a.get(), add_bn.get());
//  return BignumToString(*a);
//}

// BN_ptr ToBN(unsigned char *start, int length, unsigned int mul) {
//  auto ctx = BN_CTX_new();
//  BN_ptr result{BN_new(), ::BN_free};
//  BN_bin2bn(start, length, result.get());
//  auto mul_bn = BN_new_ptr(mul);
//  BN_mul(result.get(), result.get(), mul_bn.get(), ctx);
//  free(ctx);
//  return std::move(result);
//}

// TEST_F(BlockTaggerTest, FullFile) {
//  // Blocks{"abcd", "efgh", "i"}
//  std::stringstream s{"abcdefghi"};
//  auto s_ptr = (unsigned char *)s.str().data();

//  file_tag.num_sectors = 2;
//  file_tag.sector_size = 2;
//  file_tag.alphas.emplace_back(BN_new_ptr(2));
//  file_tag.alphas.emplace_back(BN_new_ptr(4));
//  auto t = GetBlockTagger(s);

//  std::vector<proto::BlockTag> tags;
//  while (t.HasNext()) {
//    tags.push_back(t.GetNext());
//  }

//  std::vector<proto::BlockTag> expected;
//  proto::BlockTag block;
//  block.set_index(0);
//  block.set_sigma(Result(ToBN(s_ptr, 2, 2), ToBN(s_ptr + 2, 2, 4), 0));
//  expected.push_back(block);
//  block.set_index(1);
//  block.set_sigma(Result(ToBN(s_ptr + 4, 2, 2), ToBN(s_ptr + 6, 2, 4), 1));
//  expected.push_back(block);
//  block.set_index(2);
//  block.set_sigma(Result(ToBN(s_ptr + 8, 1, 2), BN_new_ptr(0), 2));
//  expected.push_back(block);

//  ExpectProtosEqual(expected, tags);
//}

// int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
