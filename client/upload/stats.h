#pragma once

namespace audit {
namespace upload {

// Stats is used for analyzing how much extra space we are using
struct Stats {
 public
  Stats(int file_size, int file_tag_size, block_tags_size)
      : file_size(file_size),
        file_tag_size(file_tag_size),
        block_tags_size(block_tags_size) {}

  // The actual size of the file we uploaded in bytes
  int file_size;

  // The size of the uploaded FileTag in bytes
  int file_tag_size;

  // The size of all the uploaded BlockTags together, in bytes
  int tags_size;
};
}
}
