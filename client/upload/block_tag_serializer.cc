#include "audit/client/upload/block_tag_serializer.h"

#include <assert.h>
#include <fstream>

namespace audit {
namespace upload {

const std::string BlockTagSerializer::files_dir{
    "/users/adambalogh/Developer/audit/files_dir/"};

void BlockTagSerializer::Flush() {
  if (end == 0) return;
  out_file_.write((char*)buffer.data(), end);
  end = 0;
}

proto::BlockTagMap BlockTagSerializer::Done() {
  Flush();
  return block_tag_map_;
}

void BlockTagSerializer::Add(const proto::BlockTag& tag) {
  if (end + tag.ByteSize() > buffer.size()) {
    Flush();
  }
  std::string serialized;
  serialized.resize(tag.ByteSize());
  tag.SerializeToString(&serialized);

  std::copy(serialized.begin(), serialized.end(), buffer.begin() + end);
  end += tag.ByteSize();

  offset += tag.ByteSize();

  block_tag_map_.add_offset(offset);
  block_tag_map_.add_index(tag.index());

  assert(end <= buffer.size());
}
}
}
