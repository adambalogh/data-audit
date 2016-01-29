#include "gtest/gtest.h"
#include "audit/client/upload/block_tag_serializer.h"

#include "audit/proto/cpor.pb.h"

using namespace audit;
using namespace audit::upload;

TEST(BlockTagSerializer, WriteOne) {
  BlockTagSerializer serializer{"hello"};

  proto::BlockTag tag;
  tag.set_index(10);

  serializer.Add(tag);
  serializer.Done();
}

TEST(BlockTagSerializer, BlockTagMap) {
  BlockTagSerializer serializer{"hello"};

  proto::BlockTag tag;
  tag.set_index(10);

  proto::BlockTag tag2;
  tag2.set_index(1);
  tag2.set_sigma("hwer");

  serializer.Add(tag);
  serializer.Add(tag2);
  auto block_tag_map = serializer.Done();

  EXPECT_EQ(2, block_tag_map.end_size());
  EXPECT_EQ(2, block_tag_map.index_size());
  EXPECT_EQ(10, block_tag_map.index(0));
  EXPECT_EQ(tag.ByteSize(), block_tag_map.end(0));
  EXPECT_EQ(1, block_tag_map.index(1));
  EXPECT_EQ(tag.ByteSize() + tag2.ByteSize(), block_tag_map.end(1));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
