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

// A FileTag must be created for each file we want to tag and upload.
// It holds important information about the file and parameters to the tagging.
//
// After we are done tagging the file. It should be serialized to a proto, by
// using the PrivateProto() method and saved along with the file and tags, as
// it will be used for verification later.
//
class FileTag {
 public:
  FileTag(std::istream& file, unsigned long num_sectors, size_t sector_size,
          BN_ptr p, RandomNumberGenerator* random_gen);

  proto::PrivateFileTag PrivateProto() const;
  proto::PublicFileTag PublicProto() const;

  std::istream& file() const { return file_; }
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
  void MakeAlphas(RandomNumberGenerator* random_gen);

  std::istream& file_;

  // As documented in proto/cpor.proto
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
