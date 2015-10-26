#include "gtest/gtest.h"
#include <sstream>

#include "file_tagger.h"

using namespace audit;

TEST(FileTagger, EmptyFile) {
  std::stringstream s;
  FileTagger t{s, 10, 10};
  EXPECT_EQ(false, t.HasNext());
}

TEST(FileTagger, IsValid) {
  std::stringstream s{"a"};
  FileTagger t{s, 1, 1};
  EXPECT_EQ(true, t.HasNext());
}

TEST(FileTagger, SingleLetter) {
  std::stringstream s{"a"};
  FileTagger t{s, 10, 10};
  auto tag = t.GetNext();
  EXPECT_EQ(static_cast<long>('a'), tag.sigma.ConvertToLong());
}

TEST(FileTagger, BecomesInvalid) {
  std::stringstream s{"abc"};
  FileTagger t{s, 10, 10};
  t.GetNext();
  EXPECT_EQ(false, t.HasNext());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
