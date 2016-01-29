#pragma once

#include <math.h>

namespace audit {
namespace upload {

// Stats is used for keeping track of how much space is used to store the
// file and its tags
struct Stats {
 public:
  Stats() {}

  Stats(size_t file_size, size_t file_tag_size, size_t block_tags_size)
      : file_size(file_size),
        file_tag_size(file_tag_size),
        block_tags_size(block_tags_size) {}

  // Returns string representation of stats
  std::string String() const {
    std::string out;
    out += "File Size: " + std::to_string(ToMB(file_size)) + " MB\n";
    out += "FileTag Size: " + std::to_string(ToMB(file_tag_size)) + " MB\n";
    out += "BlockTags Size: " + std::to_string(ToMB(block_tags_size)) + " MB\n";
    return out;
  }

  // The actual size of the file we uploaded in bytes
  size_t file_size{0};

  // The size of the uploaded FileTag in bytes
  size_t file_tag_size{0};

  // The size of all the uploaded BlockTags together, in bytes
  size_t block_tags_size{0};

 private:
  // Returns the number of Megabytes the given bytes are equal to, rounded to 3
  // decimal places
  static double ToMB(size_t bytes) {
    return static_cast<double>(floor(bytes / 1000)) / 1000;
  }
};
}
}
