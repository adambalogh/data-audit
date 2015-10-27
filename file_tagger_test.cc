#include "gtest/gtest.h"

#include <sstream>
#include <vector>

#include "file_tagger.h"

#include "audit/third_party/cryptopp/integer.h"

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

ConstantNumberGenerator c_gen;
CryptoPP::Integer p{256203221};

TEST(FileTagger, EmptyFile) {
  std::stringstream s;
  FileTagger t{s, 10, 10, p, c_gen};
  EXPECT_EQ(false, t.HasNext());
}

TEST(FileTagger, NotEmptyFile) {
  std::stringstream s{"a"};
  FileTagger t{s, 1, 1, p, c_gen};
  EXPECT_EQ(true, t.HasNext());
}

TEST(FileTagger, SingleLetter) {
  std::stringstream s{"a"};
  FileTagger t{s, 10, 10, p, c_gen};
  auto tag = t.GetNext();
  EXPECT_EQ(0, tag.index);
  EXPECT_EQ(static_cast<long>('a'), tag.sigma);
}

TEST(FileTagger, BecomesInvalid) {
  std::stringstream s{"abc"};
  FileTagger t{s, 10, 10, p, c_gen};
  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

TEST(FileTagger, MultipleBlocks) {
  std::stringstream s{"abc"};
  DummyNumberGenerator gen{{10}};
  FileTagger t{s, 1, 1, p, gen};

  std::vector<BlockTag> tags;
  while (t.HasNext()) {
    tags.push_back(t.GetNext());
  }

  std::vector<BlockTag> expected{
      BlockTag{0, CryptoPP::Integer{'a'} * 10},
      BlockTag{1, CryptoPP::Integer{'b'} * 10},
      BlockTag{2, CryptoPP::Integer{'c'} * 10},
  };
  EXPECT_EQ(expected, tags);
}

TEST(FileTagger, FileTag) {
  std::stringstream s{"abc"};
  FileTagger t{s, 1, 1, c_gen};
  while (t.HasNext()) {
    t.GetNext();
  }
  auto file_tag = t.GetFileTag();
  FileTag expected;
  expected.num_blocks = 3;
  expected.num_sectors = 1;
  expected.sector_size = 1;
  expected.p = p;
  epxected.alphas = {1};
  EXPECT_EQ(expected, file_tag);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
