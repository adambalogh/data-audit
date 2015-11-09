#pragma once

#include <iostream>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

// Fetches individual blocks and block tags of a file
class Fetcher {
 public:
  // Returns the content of the block at index
  virtual std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) = 0;
  //
  // Returns the BlockTag for the block at index
  virtual proto::BlockTag FetchBlockTag(unsigned long index) = 0;

  virtual ~Fetcher() {}
};
}
