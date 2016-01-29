#include "audit/client/upload/block_tag_serializer.h"

#include <assert.h>
#include <fstream>
#include <iostream>

namespace audit {
namespace upload {

const std::string BlockTagSerializer::files_dir{
    "/users/adambalogh/Developer/audit/files_dir/tags_files/"};

void BlockTagSerializer::Flush() {
  size_t required_size = 0;
  for (auto& tag : tags_) {
    required_size += tag.ByteSize();
  }

  std::vector<unsigned char> buffer(required_size);
  size_t buffer_end = 0;

  for (auto& tag : tags_) {
    tag.SerializeToArray(buffer.data() + buffer_end, buffer.size());
    buffer_end += tag.ByteSize();
  }
  assert(buffer_end == buffer.size());

  out_file_.write((char*)buffer.data(), buffer.size());

  tags_.clear();
  buffer_size_ = 0;
}

proto::BlockTagMap BlockTagSerializer::Done() {
  Flush();
  out_file_.close();
  return block_tag_map_;
}

void BlockTagSerializer::Add(const proto::BlockTag& tag) {
  if (buffer_size_ + tag.ByteSize() > 1000 * 1000 * 2) {
    Flush();
  }

  tags_.push_back(tag);

  buffer_size_ += tag.ByteSize();
  file_end_ += tag.ByteSize();

  block_tag_map_.add_index(tag.index());
  block_tag_map_.add_end(file_end_);
}
}
}
