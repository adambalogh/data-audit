#pragma once

#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

// Fetches individual blocks and block tags of a file
class Fetcher {
 public:
  Fetcher(const proto::FileTag& file_tag) : file_tag_(file_tag) {}

  // Returns the content of the block at index
  virtual std::basic_iostream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) = 0;
  //
  // Returns the BlockTag for the block at index
  virtual proto::BlockTag FetchBlockTag(unsigned long index) = 0;

  virtual ~Fetcher() {}

 private:
  size_t BlockSize() const {
    return file_tag_.num_sectors() * file_tag_.sector_size();
  }

  const proto::FileTag& file_tag_;
};
}
