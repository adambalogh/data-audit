#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "audit/proto/cpor.pb.h"

namespace audit {

// Fetches individual blocks and tags of a file
class Fetcher {
 public:
  // Returns the content of the block at index
  virtual std::unique_ptr<std::basic_istream<char, std::char_traits<char>>>
  FetchBlock(const proto::PublicFileTag& file_tag, unsigned long index) = 0;
  //
  // Returns the BlockTag for the block at index
  virtual proto::BlockTag FetchBlockTag(const proto::PublicFileTag& file_tag,
                                        unsigned long index) = 0;

  virtual ~Fetcher() {}
};
}
