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
      : out_file_(files_dir + "tags") {
    if (!out_file_) {
      throw std::runtime_error(
          "Could not open file to serialize BlockTags to. (" + files_dir +
          "tags" + ")");
    }
  }

  void Add(const proto::BlockTag& tag);

  proto::BlockTagMap Done();

  static const std::string files_dir;

 private:
  void Flush();

  std::ofstream out_file_;

  // TODO what if BlockTag is bigger than 2 MB
  std::array<unsigned int, 1000 * 1000 * 2> buffer;  // 2 MB

  size_t end{0};

  size_t offset{0};

  proto::BlockTagMap block_tag_map_;
};
}
}
