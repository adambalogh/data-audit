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

using namespace audit::upload;

void ExpectBlocksEqual(std::vector<proto::BlockTag> &expected,
                       std::vector<proto::BlockTag> &actual) {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected.at(i).DebugString(), actual.at(i).DebugString());
  }
}

class BlockTaggerTest : public ::testing::Test {
 protected:
  ~BlockTaggerTest() { delete context; }

  BlockTagger MakeBlockTagger(const std::string &content,
                              std::vector<unsigned int> alphas_int,
                              int num_sectors = 1, size_t sector_size = 1,
                              int p_int = 32452867) {
    auto p = BN_new_ptr(p_int);
    std::vector<BN_ptr> alphas;
    for (int i = 0; i < num_sectors; ++i) {
      alphas.push_back(BN_new_ptr(alphas_int[i]));
    }
    auto stream = std::make_unique<std::stringstream>(content);
    // TODO delete this after used
    auto file = new File{std::move(stream), ""};
    TaggingParameters params{num_sectors, sector_size};
    context = new FileContext{*file, params, std::move(alphas), std::move(p),
                              std::move(std::unique_ptr<PRF>(new DummyPRF))};
    return BlockTagger{*context};
  }

  BlockTagger MakeBlockTagger(const std::string &content,
                              unsigned int const_alpha = 1, int num_sectors = 1,
                              size_t sector_size = 1, int p_int = 32452867) {
    std::vector<unsigned int> alphas(num_sectors, const_alpha);
    return MakeBlockTagger(content, alphas, num_sectors, sector_size, p_int);
  }

  FileContext *context;
};

TEST_F(BlockTaggerTest, EmptyFile) {
  std::string s;
  auto t = MakeBlockTagger(s);
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, NotEmptyFile) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s);
  EXPECT_EQ(true, t.HasNext());
}

TEST_F(BlockTaggerTest, SingleLetter) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s, 10);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, BecomesInvalid) {
  std::string s{"abc"};
  auto t = MakeBlockTagger(s, 1, 1, 10);

  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, LargerSectorSize) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s, 10, 1, 10);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargerSectorNumber) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s, 10, 10, 1);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargeSectorSizeAndNumber) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s, 10, 10, 10);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, Modulo) {
  std::string s{"a"};
  auto t = MakeBlockTagger(s, 100, 1, 1, 11);

  int expected = (100 * static_cast<int>('a')) % 11;

  auto tag = t.GetNext();
  EXPECT_EQ(expected, StringToBignum(tag.sigma()));
}

// TODO clean up this
std::string Result(BN_ptr a, BN_ptr b, unsigned int add) {
  BN_add(a.get(), a.get(), b.get());
  auto add_bn = BN_new_ptr(add);
  BN_add(a.get(), a.get(), add_bn.get());
  return BignumToString(*a);
}

BN_ptr ToBN(unsigned char *start, int length, unsigned int mul) {
  auto ctx = BN_CTX_new();
  BN_ptr result{BN_new(), ::BN_free};
  BN_bin2bn(start, length, result.get());
  auto mul_bn = BN_new_ptr(mul);
  BN_mul(result.get(), result.get(), mul_bn.get(), ctx);
  free(ctx);
  return std::move(result);
}

TEST_F(BlockTaggerTest, FullFile) {
  // Blocks{"abcd", "efgh", "i"}
  std::string s{"abcdefghi"};
  auto s_ptr = (unsigned char *)s.data();

  auto t = MakeBlockTagger(s, {2, 4}, 2, 2);

  std::vector<proto::BlockTag> tags;
  while (t.HasNext()) {
    tags.push_back(t.GetNext());
  }

  std::vector<proto::BlockTag> expected;
  proto::BlockTag block;
  block.set_index(0);
  block.set_sigma(Result(ToBN(s_ptr, 2, 2), ToBN(s_ptr + 2, 2, 4), 0));
  expected.push_back(block);
  block.set_index(1);
  block.set_sigma(Result(ToBN(s_ptr + 4, 2, 2), ToBN(s_ptr + 6, 2, 4), 1));
  expected.push_back(block);
  block.set_index(2);
  block.set_sigma(Result(ToBN(s_ptr + 8, 1, 2), BN_new_ptr(0), 2));
  expected.push_back(block);

  ExpectBlocksEqual(expected, tags);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
