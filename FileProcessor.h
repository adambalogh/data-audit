#pragma once

#include <iostream>

#include "auditing/third_party/cryptopp/integer.h"

#include "CPORTypes.h"

namespace audit {

class FileProcessor {
 public:
  FileProcessor(std::istream& file) : file_(file) {}

  BlockTag NextTag();
  bool HasNextTag() const;
  FileTag GetFileTag();

 private:
  BlockTag GenerateTag();

  std::istream& file_;

  // The number of sectors in a block
  int num_sectors_{10};

  // The size of a block in bytes
  int sector_size_{10};

  std::vector<CryptoPP::Integer> alphas_;
};
}
