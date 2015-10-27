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

// Random number generator that returns the numbers from the passed vector, in
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
  EXPECT_EQ(static_cast<long>('a'), tag.sigma.ConvertToLong());
}

TEST(FileTagger, BecomesInvalid) {
  std::stringstream s{"abc"};
  FileTagger t{s, 10, 10, p, c_gen};
  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
