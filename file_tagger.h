#pragma once

#include <iostream>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/osrng.h"

#include "cpor_types.h"

namespace audit {

// A generic interface for random number generation
class RandomNumberGenerator {
 public:
  // Returns a random number x, where x >= 0 and x < max
  virtual CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) = 0;
  virtual ~RandomNumberGenerator() {}
};

// RandomNumberGenerator implementation using CryptoPP
class CryptoNumberGenerator : public RandomNumberGenerator {
 public:
  CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) override {
    return CryptoPP::Integer{g, 0, max};
  }

 private:
  CryptoPP::AutoSeededRandomPool g;
};

// Example usage:
//
// FileTagger f{...};
// while (f.HasNext()) {
//   BlockTag tag = f.GetNext();
//   ...
// }
// FileTag file_tag = f.GetFileTag();
//
class FileTagger {
 public:
  FileTagger(std::istream& file, int num_sectors, int sector_size,
             CryptoPP::Integer p, RandomNumberGenerator& random_gen)
      : file_(file),
        num_sectors_(num_sectors),
        sector_size_(sector_size),
        alphas_(num_sectors),
        p_(p),
        random_gen_(random_gen) {
    CheckValid();
    MakeAlphas();
  }

  // Returns the BlockTag for the next block from the file, should only be
  // called if HasNext() returns true
  BlockTag GetNext();

  // Returns false if we reached the end of the file and there are no more
  // BlockTags to be returned
  bool HasNext() const;

  // Returns the FileTag for the whole file. Should only be called after
  // HasNext() returns false, which means that the whole file has been processed
  FileTag GetFileTag();

 private:
  void CheckValid();
  void MakeAlphas();

  // Should only be called when valid_ is true
  BlockTag GenerateTag();

  // The file we are tagging
  std::istream& file_;

  // Indicates whether we can read more from the file
  bool valid_{true};

  // The total number of blocks that were processed from file
  int num_blocks_{0};

  // The number of sectors in a block
  int num_sectors_;

  // The size of a block in bytes
  int sector_size_;

  std::vector<CryptoPP::Integer> alphas_;

  CryptoPP::Integer p_;

  RandomNumberGenerator& random_gen_;
};
}
