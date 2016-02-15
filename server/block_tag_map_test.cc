#include "gtest/gtest.h"

#include "audit/server/block_tag_map.h"

using namespace audit;

TEST(BlockTagMap, FindBlock) {
  proto::BlockTagMap p_tag_map;

  p_tag_map.add_index(1);
  p_tag_map.add_end(10);

  p_tag_map.add_index(10);
  p_tag_map.add_end(20);

  BlockTagMap tag_map{p_tag_map};

  auto range = tag_map.FindBlockTag(10);
  auto expected_range = std::make_pair<size_t, size_t>(10, 20);

  EXPECT_EQ(expected_range, range);
}

TEST(BlockTagMap, FindFirstBlock) {
  proto::BlockTagMap p_tag_map;

  p_tag_map.add_index(1);
  p_tag_map.add_end(10);

  BlockTagMap tag_map{p_tag_map};

  auto range = tag_map.FindBlockTag(1);
  auto expected_range = std::make_pair<size_t, size_t>(0, 10);

  EXPECT_EQ(expected_range, range);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
