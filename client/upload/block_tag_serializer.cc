#include "audit/client/upload/block_tag_serializer.h"

#include <assert.h>
#include <fstream>
#include <iostream>

#include "audit/common.h"

namespace audit {
namespace upload {

const std::string BlockTagSerializer::files_dir{application_dir +
                                                "temp_tags_files/"};

void BlockTagSerializer::Flush() {
  size_t required_size = 0;
  for (const auto& tag : tags_) {
    required_size += tag.ByteSize();
  }
  if (required_size <= 0) {
    return;
  }

  std::vector<unsigned char> flush_buf(required_size);
  size_t flush_buf_size = 0;
  for (auto& tag : tags_) {
    tag.SerializeToArray(flush_buf.data() + flush_buf_size, flush_buf.size());
    flush_buf_size += tag.ByteSize();
  }
  assert(flush_buf_size = flush_buf.size());
  Progress(flush_buf_size);
  out_file_.write((char*)flush_buf.data(), flush_buf_size);
  tags_.clear();
  buffer_size_ = 0;
}

// TODO remove temporary block tag file after it's been stored
proto::BlockTagMap BlockTagSerializer::Done() {
  Flush();
  out_file_.close();
  return block_tag_map_;
}

void BlockTagSerializer::DeleteTempFile() const {
  std::remove(file_name_.c_str());
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
  block_tag_map_.set_max_size(
      std::max(block_tag_map_.max_size(), tag.ByteSize()));
}
}
}
