#pragma once

#include "audit/proto/cpor.pb.h"
#include "audit/client/upload/file.h"

#include <array>
#include <fstream>
#include <string>

namespace audit {
namespace upload {

// BlockTagSerizer combines all the BlockTags of a file into a single binary
// file.
//
// After we added all the BlockTags, Flush() must be called to write all the
// tags to the file.
//
class BlockTagSerializer {
 public:
  BlockTagSerializer(const std::string& file_full_path)
      : file_name_(files_dir + "tags"), out_file_(file_name_) {
    if (!out_file_) {
      throw std::runtime_error(
          "Could not open file to serialize BlockTags to. (" + files_dir +
          "tags" + ")");
    }
  }

  // Each BlockTag of the file must be added
  void Add(const proto::BlockTag& tag);

  // Must be called when we added all the BlockTags
  proto::BlockTagMap Done();

  // Returns the full path and name of the file where the serialized tags are
  // stored
  std::string GetFileName() const { return file_name_; }

  // The directory under which all the tag files are stored
  static const std::string files_dir;

 private:
  // Writes the content of the buffer to out_file
  void Flush();

  // Name of the file where the tags are written
  const std::string file_name_;

  std::ofstream out_file_;

  // TODO what if BlockTag is bigger than 2 MB
  std::array<unsigned int, 1000 * 1000 * 2> buffer;  // 2 MB

  // Indicates where we can write in the buffer
  size_t buffer_end{0};

  // Indicates the current size of out_file
  size_t file_end{0};

  proto::BlockTagMap block_tag_map_;
};
}
}
