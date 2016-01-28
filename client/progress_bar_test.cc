#include "gtest/gtest.h"

#include "audit/client/progress_bar.h"

using namespace audit;

TEST(ProgressBar, New) {
  ProgressBar b{10};
  ASSERT_FALSE(b.Done());
  ASSERT_EQ(0, b.Percentage());
}

TEST(ProgessBar, OneStep) {
  ProgressBar b{100};
  b.Progress(1);
  ASSERT_EQ(1, b.Percentage());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
