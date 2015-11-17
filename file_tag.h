#pragma once

#include <array>
#include <istream>
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
  FileTag(std::istream& file, unsigned long num_sectors, size_t sector_size,
          BN_ptr p, RandomNumberGenerator* random_gen)
      : file_(file),
        num_sectors_(num_sectors),
        sector_size_(sector_size),
        p_(std::move(p)) {
    MakeAlphas(random_gen);

    // Calculate number of blocks
    file_.seekg(0, file_.end);
    auto length = file_.tellg();
    file_.seekg(0, file_.beg);
    auto block_size = sector_size_ * num_sectors_;
    num_blocks_ = length / block_size;
    if (length % block_size != 0) {
      ++num_blocks_;
    }
  }

  proto::PrivateFileTag PrivateProto() const;
  proto::PublicFileTag PublicProto() const;

  std::istream& file() { return file_; }

  unsigned long num_blocks() const { return num_blocks_; }
  unsigned long num_sectors() const { return num_sectors_; }
  size_t sector_size() const { return sector_size_; }

  const std::vector<BN_ptr>& alphas() const { return alphas_; }
  const BIGNUM* p() const { return p_.get(); }

  const std::array<unsigned char,
                   CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH>&
  hmac_key() const {
    return hmac_key_;
  }

  const std::array<unsigned char,
                   CryptoPP::HMAC<CryptoPP::SHA1>::DEFAULT_KEYLENGTH>&
  prf_key() const {
    return prf_key_;
  }

 private:
  void MakeAlphas(RandomNumberGenerator* random_gen) {
    std::generate_n(std::back_inserter(alphas_), num_sectors_, [&]() -> BN_ptr {
      return std::move(random_gen->GenerateNumber(*p_));
    });
  }

  std::istream& file_;

  unsigned long num_blocks_{0};
  unsigned long num_sectors_;
  size_t sector_size_;

  std::vector<BN_ptr> alphas_;
  BN_ptr p_;

  std::array<unsigned char, CryptoPP::HMAC<CryptoPP::SHA1>::DEFAULT_KEYLENGTH>
      hmac_key_;
  std::array<unsigned char, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH>
      prf_key_;
};
}
