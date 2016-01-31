#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace server {

// Fetcher is used to fetch individual "blocks" and BlockTags of the file
// referenced from the file_tag.
class Fetcher {
 public:
  Fetcher(const proto::PublicFileTag& file_tag) : file_tag_(file_tag) {}

  // Returns the content of the block at index, the size of the block is equal
  // to num_sectors * sector_size from the given file_tag
  virtual std::unique_ptr<std::basic_istream<unsigned char>> FetchBlock(
      const proto::PublicFileTag& file_tag, unsigned long index) = 0;
  //
  // Returns the BlockTag for the block at index
  virtual proto::BlockTag FetchBlockTag(const proto::PublicFileTag& file_tag,
                                        unsigned long index) = 0;

  virtual ~Fetcher() {}

 protected:
  const proto::PublicFileTag& file_tag_;
};
}
}
