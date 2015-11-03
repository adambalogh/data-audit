#include "gtest/gtest.h"

#include <sstream>
#include <vector>

#include "block_tagger.h"

#include "audit/third_party/cryptopp/integer.h"

#include "util.h"
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

// Random number generator that returns 1 all the time
class ConstantNumberPRF : public PRF {
 public:
  CryptoPP::Integer Encode(unsigned int i) { return CryptoPP::Integer::One(); }
};

class BlockTaggerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    file_tag.num_sectors = 10;
    file_tag.sector_size = 10;
    file_tag.p = p;
    prf.reset(new ConstantNumberPRF{});
  }

  BlockTagger GetBlockTagger(std::istream &s) {
    return BlockTagger{s, &file_tag, c_gen, std::move(prf)};
  }

  ConstantNumberGenerator c_gen;
  FileTag file_tag;
  CryptoPP::Integer p{256203221};
  std::unique_ptr<PRF> prf{new ConstantNumberPRF{}};
};

void ExpectProtosEqual(std::vector<proto::BlockTag> &expected,
                       std::vector<proto::BlockTag> &actual) {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected.at(i).DebugString(), actual.at(i).DebugString());
  }
}

TEST_F(BlockTaggerTest, EmptyFile) {
  std::stringstream s;
  auto t = GetBlockTagger(s);
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, NotEmptyFile) {
  std::stringstream s{"a"};
  file_tag.sector_size = 1;
  file_tag.num_sectors = 1;
  auto t = GetBlockTagger(s);
  EXPECT_EQ(true, t.HasNext());
}

TEST_F(BlockTaggerTest, SingleLetter) {
  std::stringstream s{"a"};
  auto t = GetBlockTagger(s);
  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index());
  EXPECT_EQ(static_cast<long>('a'), StringToCryptoInteger(tag.sigma()));
}

TEST_F(BlockTaggerTest, BecomesInvalid) {
  std::stringstream s{"abc"};
  auto t = GetBlockTagger(s);
  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

TEST_F(BlockTaggerTest, MultipleBlocks) {
  std::stringstream s{"abc"};
  auto t = GetBlockTagger(s);
  DummyNumberGenerator gen{{10}};

  std::vector<proto::BlockTag> tags;
  while (t.HasNext()) {
    tags.push_back(t.GetNext());
  }

  std::vector<proto::BlockTag> expected;

  proto::BlockTag block;
  block.set_index(0);
  block.set_sigma(CryptoIntegerToString(CryptoPP::Integer{'a'} * 10));
  expected.push_back(block);
  block.set_index(1);
  block.set_sigma(CryptoIntegerToString(CryptoPP::Integer{'b'} * 10));
  expected.push_back(block);
  block.set_index(2);
  block.set_sigma(CryptoIntegerToString(CryptoPP::Integer{'c'} * 10));
  expected.push_back(block);

  ExpectProtosEqual(expected, tags);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
