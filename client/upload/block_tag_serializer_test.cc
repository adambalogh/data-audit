#include "gtest/gtest.h"
#include "audit/client/upload/block_tag_serializer.h"

#include <fstream>
#include <iostream>

#include "audit/proto/cpor.pb.h"

using namespace audit;
using namespace audit::upload;

TEST(BlockTagSerializer, WriteOne) {
  return;
  BlockTagSerializer serializer{"hello"};

  proto::BlockTag tag;
  tag.set_index(10);
  tag.set_sigma("abcd");

  serializer.Add(tag);
  serializer.Done();

  std::ifstream tag_file{serializer.FileName(), std::ifstream::binary};

  proto::BlockTag read_tag;
  EXPECT_TRUE(read_tag.ParseFromIstream(&tag_file));

  EXPECT_EQ(tag.index(), read_tag.index());
  EXPECT_EQ(tag.sigma(), read_tag.sigma());
}

TEST(BlockTagSerializer, WriteThree) {
  BlockTagSerializer serializer{"hello"};

  proto::BlockTag tag1;
  tag1.set_index(10);
  tag1.set_sigma("abcd");

  proto::BlockTag tag2;
  tag2.set_index(3);
  tag2.set_sigma("agerfs");

  proto::BlockTag tag3;
  tag3.set_index(0);
  tag3.set_sigma("a");

  std::vector<proto::BlockTag> tags{tag1, tag2, tag3};
  for (auto& tag : tags) {
    serializer.Add(tag);
  }
  serializer.Done();

  std::ifstream tag_file{serializer.FileName(), std::ifstream::binary};

  size_t start = 0;
  for (auto& tag : tags) {
    auto serialized = tag.SerializeAsString();
    std::vector<unsigned char> expected_binary(serialized.begin(),
                                               serialized.end());

    tag_file.seekg(start);
    start += tag.ByteSize();
    std::vector<unsigned char> binary(tag.ByteSize());
    tag_file.read((char*)binary.data(), tag.ByteSize());

    EXPECT_EQ(expected_binary, binary);
  }
}

TEST(BlockTagSerializer, BlockTagMap) {
  return;
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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
