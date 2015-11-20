#include "gtest/gtest.h"

#include <sstream>

#include "openssl/bn.h"

#include "audit/file_tag.h"

using namespace audit;

// Random number generator that returns 1 all the time
template <int n>
class ConstantNumberGenerator : public RandomNumberGenerator {
 public:
  BN_ptr GenerateNumber(const BIGNUM &) {
    BN_ptr num{BN_new(), ::BN_free};
    BN_set_word(num.get(), n);
    return std::move(num);
  }
};

// Random number generator that returns the numbers from the given vector, in
// sequence
class DummyNumberGenerator : public RandomNumberGenerator {
 public:
  DummyNumberGenerator(std::vector<int> nums) : nums_(nums) {}
  BN_ptr GenerateNumber(const BIGNUM &) override {
    BN_ptr num{BN_new(), ::BN_free};
    BN_set_word(num.get(), nums_[index++]);
    return std::move(num);
  }

 private:
  int index{0};
  std::vector<int> nums_;
};

TEST(FileTag, NumBlocks) {
  std::stringstream file{"aaaaaaaaa"};
  ConstantNumberGenerator<1> g;
  BN_ptr p{BN_new(), ::BN_free};
  FileTag tag{file, 2, 1, std::move(p), &g};

  EXPECT_EQ(5, tag.num_blocks());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
