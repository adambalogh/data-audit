#pragma once

#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

class Fetcher {
 public:
  Fetcher(const proto::FileTag& file_tag) : file_tag_(file_tag) {}

  virtual std::basic_iostream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) = 0;
  virtual proto::BlockTag FetchBlockTag(unsigned long index) = 0;

 private:
  proto::FileTag& file_tag_;
}
}
