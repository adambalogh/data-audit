#pragma once

#include <array>
#include <vector>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"

#include "util.h"

namespace audit {

typedef unsigned char byte;

class FileTag {
 public:
  FileTag() {}
  FileTag(unsigned long num_sectors, int sector_size, CryptoPP::Integer p,
          RandomNumberGenerator* random_gen)
      : num_sectors(num_sectors),
        sector_size(sector_size),
        p(p),
        random_gen(random_gen) {
    MakeAlphas();
  }
  void set_num_blocks(unsigned long n) { num_blocks = n; }

  void MakeAlphas() {
    alphas.resize(num_sectors);
    std::generate(std::begin(alphas), std::end(alphas),
                  [&]() { return random_gen->GenerateNumber(p); });
  }

  unsigned long num_blocks{0};
  unsigned long num_sectors;
  size_t sector_size;
  std::vector<CryptoPP::Integer> alphas;
  CryptoPP::Integer p;
  RandomNumberGenerator* random_gen;
};

struct SecretKeys {
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH> prf_key;
};
}
