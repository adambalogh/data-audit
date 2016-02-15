#include "gtest/gtest.h"

#include "audit/client/upload/progress_bar.h"

using namespace audit;

TEST(ProgressBar, New) {
  ProgressBar b{10};
  ASSERT_EQ(0, b.Percentage());
  ASSERT_FALSE(b.Done());
}

TEST(ProgessBar, OneStep) {
  ProgressBar b{100};
  b.Progress(1);
  ASSERT_EQ(1, b.Percentage());
}

TEST(ProgressBar, Complete) {
  ProgressBar b{100};
  b.Progress(50);
  b.Progress(50);
  ASSERT_EQ(100, b.Percentage());
  ASSERT_TRUE(b.Done());
}

TEST(ProgressBar, Fraction) {
  ProgressBar b{3};

  b.Progress(1);
  ASSERT_EQ(33, b.Percentage());

  b.Progress(1);
  ASSERT_EQ(66, b.Percentage());

  b.Progress(1);
  ASSERT_EQ(100, b.Percentage());
  ASSERT_TRUE(b.Done());
}

TEST(ProgressBar, Over) {
  ProgressBar b{5};
  b.Progress(5);
  b.Progress(2);

  ASSERT_EQ(100, b.Percentage());
}

TEST(ProgressBar, Callback) {
  int value{0};
  ProgressBar b{10, [&value](int p) {
    EXPECT_TRUE(p > value);
    value = p;
  }};

  b.Progress(1);
  b.Progress(5);
  b.Progress(4);
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
