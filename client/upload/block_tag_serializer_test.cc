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

  serializer.Add(tag);
  auto block_tag_map = serializer.Done();

  proto::BlockTagMap expected;
  expected.add_index(10);
  expected.add_offset(tag.ByteSize());

  EXPECT_EQ(expected.DebugString(), block_tag_map.DebugString());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
