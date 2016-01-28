#pragma once

namespace audit {
namespace upload {

// Stats is used for keeping track of how much space we are using to store the
// file and its tags
struct Stats {
 public:
  Stats() {}

  Stats(size_t file_size, size_t file_tag_size, size_t block_tags_size)
      : file_size(file_size),
        file_tag_size(file_tag_size),
        block_tags_size(block_tags_size) {}

  // The actual size of the file we uploaded in bytes
  size_t file_size{0};

  // The size of the uploaded FileTag in bytes
  size_t file_tag_size{0};

  // The size of all the uploaded BlockTags together, in bytes
  size_t block_tags_size{0};
};
}
}
