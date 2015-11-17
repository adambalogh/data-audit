#pragma once

#include <array>
#include <vector>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"
#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/common.h"
#include "audit/util.h"

namespace audit {

class FileTag {
 public:
  FileTag() : p(BN_ptr{BN_new(), ::BN_free}) {}

  FileTag(unsigned long num_sectors, size_t sector_size, BN_ptr p,
          RandomNumberGenerator* random_gen)
      : num_sectors(num_sectors), sector_size(sector_size), p(std::move(p)) {
    MakeAlphas(random_gen);
  }

  proto::PrivateFileTag PrivateProto() const;
  proto::PublicFileTag PublicProto() const;

  unsigned long num_blocks{0};
  unsigned long num_sectors;
  size_t sector_size;

  std::vector<BN_ptr> alphas;
  BN_ptr p;

  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH>
      hmac_key;
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH> prf_key;

 private:
  void MakeAlphas(RandomNumberGenerator* random_gen) {
    std::generate_n(std::back_inserter(alphas), num_sectors, [&]() -> BN_ptr {
      return std::move(random_gen->GenerateNumber(*p));
    });
  }
};
}
