#pragma once

#include <iostream>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/osrng.h"

#include "cpor_types.h"

namespace audit {

class RandomNumberGenerator {
 public:
  // Returns a random number x, where x >= 0 and x < max
  virtual CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) = 0;
  virtual ~RandomNumberGenerator() {}
};

class CryptoNumberGenerator : public RandomNumberGenerator {
 public:
  CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) override {
    return CryptoPP::Integer{g, 0, max};
  }

 private:
  CryptoPP::AutoSeededRandomPool g;
};

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

  BlockTag GetNext() { return GenerateTag(); }
  bool HasNext() { return valid_; }

  FileTag GetFileTag();

 private:
  void CheckValid();
  void MakeAlphas();
  BlockTag GenerateTag();

  std::istream& file_;
  bool valid_{true};

  // The number of sectors in a block
  int num_sectors_;

  // The size of a block in bytes
  int sector_size_;

  std::vector<CryptoPP::Integer> alphas_;
  CryptoPP::Integer p_;
  RandomNumberGenerator& random_gen_;
};
}
