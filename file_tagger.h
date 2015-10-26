#pragma once

#include <iostream>

#include "audit/third_party/cryptopp/integer.h"

#include "cpor_types.h"

namespace audit {

class FileTagger {
 public:
  FileTagger(std::istream& file) : file_(file) { MakeAlphas(); }

  BlockTag NextTag();
  bool HasNextTag();
  FileTag GetFileTag();

 private:
  void MakeAlphas();
  BlockTag GenerateTag();

  std::istream& file_;

  // The number of sectors in a block
  int num_sectors_{10};

  // The size of a block in bytes
  int sector_size_{10};

  std::vector<CryptoPP::Integer> alphas_;
};
}
