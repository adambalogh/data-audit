#pragma once

#include <string>
#include <sstream>

#include "audit/client/stats.h"

namespace audit {
namespace upload {

// Stats is used for keeping track of how much space is used to store the
// file and its tags
struct Stats : public ::audit::Stats {
 public:
  Stats() {}

  Stats(size_t file_size, size_t file_tag_size, size_t block_tags_size)
      : file_size(file_size),
        file_tag_size(file_tag_size),
        block_tags_size(block_tags_size) {}

  // Returns string representation of stats
  std::string to_string() const {
    std::stringstream repr;
    repr << "File Size: " << ToMB(file_size) << " MB\n";
    repr << "FileTag Size: " << ToMB(file_tag_size) << " MB\n";
    repr << "BlockTags Size: " << ToMB(block_tags_size) << " MB\n";
    return repr.str();
  }

  // The actual size of the file we uploaded in bytes
  size_t file_size{0};

  // The size of the uploaded FileTag in bytes
  size_t file_tag_size{0};

  // The size of all the uploaded BlockTags together, in bytes
  size_t block_tags_size{0};
};
}
}
