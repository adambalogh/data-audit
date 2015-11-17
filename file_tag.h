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
  // TODO fix this
  FileTag()
      : p([]() {
          BN_ptr n{BN_new(), ::BN_free};
          BN_set_word(n.get(), 0);
          return std::move(n);
        }()) {}

  FileTag(unsigned long num_sectors, int sector_size, BN_ptr p,
          RandomNumberGenerator* random_gen)
      : num_sectors(num_sectors), sector_size(sector_size), p(std::move(p)) {
    MakeAlphas(random_gen);
  }

  void MakeAlphas(RandomNumberGenerator* random_gen) {
    std::generate_n(std::back_inserter(alphas), num_sectors, [&]() -> BN_ptr {
      return std::move(random_gen->GenerateNumber(*p));
    });
  }

  proto::PrivateFileTag PrivateProto() const;
  proto::PublicFileTag PublicProto() const;

  unsigned long num_blocks{0};
  unsigned long num_sectors;
  size_t sector_size;
  std::vector<BN_ptr> alphas;
  BN_ptr p;
};

struct SecretKeys {
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH> prf_key;
};
}
