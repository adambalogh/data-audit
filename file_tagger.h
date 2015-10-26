#pragma once

#include <iostream>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"

#include "cpor_types.h"

namespace audit {

class FileTagger {
 public:
  FileTagger(std::istream& file, int num_sectors, int sector_size)
      : file_(file),
        num_sectors_(num_sectors),
        sector_size_(sector_size),
        alphas_(num_sectors) {
    if (file.peek() == std::char_traits<char>::eof()) {
      valid_ = false;
      return;
    }
    MakeAlphas();
  }

  BlockTag GetNext() { return GenerateTag(); }
  bool HasNext() { return valid_; }

  FileTag GetFileTag();

 private:
  void MakeAlphas();
  BlockTag GenerateTag();

  std::istream& file_;
  bool valid_{true};

  // The number of sectors in a block
  int num_sectors_;

  // The size of a block in bytes
  int sector_size_;

  std::vector<CryptoPP::Integer> alphas_;
};
}
