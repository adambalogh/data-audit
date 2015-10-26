#include "gtest/gtest.h"
#include <sstream>

#include "file_tagger.h"

using namespace audit;

TEST(FileTagger, SingleLetter) {
  std::stringstream s{"a"};
  audit::FileTagger t{s, 1, 1};
  auto tag = t.GetNext();
  EXPECT_EQ(static_cast<int>('a'), tag.sigma);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
