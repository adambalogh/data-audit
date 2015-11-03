#include "gtest/gtest.h"
#include "block_tagger.h"

#include <sstream>
#include <string>
#include <vector>

#include "cryptopp/integer.h"

#include "util.h"
#include "proto/cpor.pb.h"
#include "prf.h"

using namespace audit;

// Random number generator that returns 1 all the time
class ConstantNumberGenerator : public RandomNumberGenerator {
 public:
  CryptoPP::Integer GenerateNumber(const CryptoPP::Integer &) {
    return CryptoPP::Integer::One();
  }
};

// Random number generator that returns the numbers from the given vector, in
// sequence
class DummyNumberGenerator : public RandomNumberGenerator {
 public:
  DummyNumberGenerator(std::vector<int> nums) : nums_(nums) {}
  CryptoPP::Integer GenerateNumber(const CryptoPP::Integer &) override {
    return CryptoPP::Integer{nums_.at(index++)};
  }

 private:
  int index{0};
  std::vector<int> nums_;
};

void ExpectProtosEqual(std::vector<proto::BlockTag> &expected,
                       std::vector<proto::BlockTag> &actual) {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected.at(i).DebugString(), actual.at(i).DebugString());
  }
}

class DummyPRF : public PRF {
 public:
  CryptoPP::Integer Encode(unsigned int i) { return CryptoPP::Integer{i}; }
};

class BlockTaggerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    file_tag.num_sectors = 10;
    file_tag.sector_size = 10;
    file_tag.p = p;
    prf.reset(new DummyPRF{});
  }

  BlockTagger GetBlockTagger(std::istream &s) {
    return BlockTagger{s, &file_tag, std::move(prf)};
  }

  FileTag file_tag;
  CryptoPP::Integer p{11111111111};
  std::unique_ptr<PRF> prf{new DummyPRF{}};
};

TEST_F(BlockTaggerTest, EmptyFile) {
  std::stringstream s;
  auto t = GetBlockTagger(s);
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, NotEmptyFile) {
  std::stringstream s{"a"};
  auto t = GetBlockTagger(s);
  EXPECT_EQ(true, t.HasNext());
}

TEST_F(BlockTaggerTest, SingleLetter) {
  std::stringstream s{"a"};

  file_tag.alphas = {10};
  file_tag.num_sectors = 1;
  file_tag.sector_size = 1;
  auto t = GetBlockTagger(s);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargerSectorSize) {
  std::stringstream s{"a"};

  file_tag.alphas = {10};
  file_tag.num_sectors = 1;
  file_tag.sector_size = 10;
  auto t = GetBlockTagger(s);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargerSectorNumber) {
  std::stringstream s{"a"};

  file_tag.alphas = {10, 10};
  file_tag.num_sectors = 10;
  file_tag.sector_size = 1;
  auto t = GetBlockTagger(s);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, LargeSectorSizeAndNumber) {
  std::stringstream s{"a"};

  file_tag.alphas = {10};
  file_tag.num_sectors = 10;
  file_tag.sector_size = 10;
  auto t = GetBlockTagger(s);

  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a') * 10, StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, BecomesInvalid) {
  std::stringstream s{"abc"};

  file_tag.alphas = {1};
  file_tag.num_sectors = 1;
  auto t = GetBlockTagger(s);

  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, Modulo) {
  std::stringstream s{"a"};

  file_tag.alphas = {100};
  file_tag.p = 11;
  auto t = GetBlockTagger(s);

  int expected = (100 * static_cast<int>('a')) % 11;

  auto tag = t.GetNext();
  EXPECT_EQ(CryptoPP::Integer{expected}, StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, NumBlocks) {
  std::stringstream s{"aaaaa"};

  file_tag.num_blocks = 100;
  file_tag.alphas = {1, 1};
  file_tag.num_sectors = 2;
  file_tag.sector_size = 1;
  auto t = GetBlockTagger(s);

  while (t.HasNext()) {
    t.GetNext();
  }

  EXPECT_EQ(3, file_tag.num_blocks);
}

TEST_F(BlockTaggerTest, FullFile) {
  // Blocks{"abcd", "efgh", "i"}
  std::stringstream s{"abcdefghi"};
  auto s_ptr = (unsigned char *)s.str().data();

  file_tag.num_sectors = 2;
  file_tag.sector_size = 2;
  file_tag.alphas = {2, 4};
  auto t = GetBlockTagger(s);

  std::vector<proto::BlockTag> tags;
  while (t.HasNext()) {
    tags.push_back(t.GetNext());
  }

  std::vector<proto::BlockTag> expected;
  proto::BlockTag block;
  block.set_index(0);
  block.set_sigma(CryptoIntegerToString((CryptoPP::Integer{s_ptr, 2} * 2) +
                                        (CryptoPP::Integer{s_ptr + 2, 2} * 4) +
                                        0));
  expected.push_back(block);
  block.set_index(1);
  block.set_sigma(CryptoIntegerToString((CryptoPP::Integer{s_ptr + 4, 2} * 2) +
                                        (CryptoPP::Integer{s_ptr + 6, 2} * 4) +
                                        1));
  expected.push_back(block);
  block.set_index(2);
  block.set_sigma(
      CryptoIntegerToString((CryptoPP::Integer{s_ptr + 8, 1} * 2) + 2));
  expected.push_back(block);

  ExpectProtosEqual(expected, tags);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
