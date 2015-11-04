#pragma once

#include <array>
#include <vector>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"
#include "openssl/bn.h"

#include "common.h"
#include "util.h"

namespace audit {

class FileTag {
 public:
  FileTag(unsigned long num_sectors, int sector_size, BN_ptr p,
          RandomNumberGenerator* random_gen)
      : num_sectors(num_sectors),
        sector_size(sector_size),
        p(std::move(p)),
        random_gen(random_gen) {
    MakeAlphas();
  }

  void MakeAlphas() {
    std::generate_n(std::back_inserter(alphas), num_sectors, [&]() -> BN_ptr {
      return std::move(random_gen->GenerateNumber(*p));
    });
  }

  unsigned long num_blocks{0};
  unsigned long num_sectors;
  size_t sector_size;
  std::vector<BN_ptr> alphas;
  BN_ptr p;
  RandomNumberGenerator* random_gen;
};

struct SecretKeys {
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH> prf_key;
};
}
