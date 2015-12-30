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

class BlockTaggerTest : public ::testing::Test {
 protected:
  ~BlockTaggerTest() { delete file; }

  void BasicFile(std::istream &stream, int num_sectors, size_t sector_size,
                 int p_int, RandomNumberGenerator *gen) {
    delete file;
    BN_ptr p{BN_new(), ::BN_free};
    BN_set_word(p.get(), p_int);
    std::vector<BN_ptr> alphas;
    for (int i = 0; i < num_sectors; ++i) {
      alphas.push_back(gen->GenerateNumber(*p));
    }
    // TODO remove random number generators
    file = new File{stream, num_sectors, sector_size, std::move(alphas),
                    std::move(p)};
  }

  BlockTagger MakeBlockTagger(std::istream &stream, RandomNumberGenerator *gen,
                              int num_sectors = 1, size_t sector_size = 1,
                              int p = 32452867) {
    BasicFile(stream, num_sectors, sector_size, p, gen);
    return std::move(
        BlockTagger{*file, std::move(std::unique_ptr<PRF>(new DummyPRF))});
  }

  ConstantNumberGenerator<1> const_gen;
  File *file{nullptr};
  int p_const = 32452867;
};

TEST_F(BlockTaggerTest, EmptyFile) {
  std::stringstream s;
  auto t = MakeBlockTagger(s, &const_gen);
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, NotEmptyFile) {
  std::stringstream s{"a"};
  auto t = MakeBlockTagger(s, &const_gen);
  EXPECT_EQ(true, t.HasNext());
}

TEST_F(BlockTaggerTest, SingleLetter) {
  std::stringstream s{"a"};
  ConstantNumberGenerator<10> gen;
  auto t = MakeBlockTagger(s, &gen, 1, 1, p_const);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, BecomesInvalid) {
  std::stringstream s{"abc"};
  auto t = MakeBlockTagger(s, &const_gen, 1, 10, p_const);

  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, LargerSectorSize) {
  std::stringstream s{"a"};
  ConstantNumberGenerator<10> gen;
  auto t = MakeBlockTagger(s, &gen, 1, 10, p_const);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargerSectorNumber) {
  std::stringstream s{"a"};
  ConstantNumberGenerator<10> gen;
  auto t = MakeBlockTagger(s, &gen, 10, 1, p_const);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargeSectorSizeAndNumber) {
  std::stringstream s{"a"};
  ConstantNumberGenerator<10> gen;
  auto t = MakeBlockTagger(s, &gen, 10, 10, p_const);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToBignum(tag.sigma()));
}

TEST_F(BlockTaggerTest, Modulo) {
  std::stringstream s{"a"};
  ConstantNumberGenerator<100> gen;
  auto t = MakeBlockTagger(s, &gen, 1, 1, 11);

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
  std::stringstream s{"abcdefghi"};
  auto s_ptr = (unsigned char *)s.str().data();

  DummyNumberGenerator gen{{2, 4}};
  auto t = MakeBlockTagger(s, &gen, 2, 2, p_const);

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

  ExpectProtosEqual(expected, tags);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
