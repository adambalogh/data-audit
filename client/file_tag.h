#pragma once

#include <array>
#include <istream>
#include <vector>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

// A FileTag must be created for each file we want to tag and upload.
// It holds important information about the file and parameters to the tagging.
//
// The complexity of verifying a this file's integrity can be adjusted by
// num_sectors and sector_size parameters. Storage overhead on the server side
// can be lowered by increasing num_sectors. The storage overhead on the server
// is equal to (1 / num_sectors), relative to the file's size. On the other
// hand, as we increase num_sectors, the size of data that needs to be
// retrieved for each verification increases linearly. It is up to the user to
// decide which factor is more important.
//
// After we are done tagging the file. It should be serialized to a proto, by
// using the PrivateProto() method and saved along with the file and tags, as
// it will be used for verification later.
//
class FileTag {
 public:
  FileTag(std::istream& file, const std::string& file_name,
          unsigned long num_sectors, size_t sector_size,
          std::vector<BN_ptr> alphas, BN_ptr p);

  std::istream& file() const { return file_; }

  unsigned long num_blocks() const { return num_blocks_; }
  unsigned long num_sectors() const { return num_sectors_; }
  size_t sector_size() const { return sector_size_; }
  unsigned long block_size() const {
    return num_sectors_ * static_cast<unsigned long>(sector_size_);
  }

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

  proto::PrivateFileTag PrivateProto() const;
  proto::PublicFileTag PublicProto() const;

 private:
  void CalculateNumBlocks();

  // The file we want to tag
  std::istream& file_;

  // The name of the file we're tagging
  std::string file_name_;

  // The number of blocks in a file
  unsigned long num_blocks_{0};

  // The number of sectors in a block
  unsigned long num_sectors_;

  // The size of a sector in bytes
  size_t sector_size_;

  // List of numbers that are used for generating tags,
  // The size of this list must be equal to the number of sectors
  std::vector<BN_ptr> alphas_;

  // p_ is a large prime number, its size should be equal to sector_size_
  BN_ptr p_;

  std::array<unsigned char, CryptoPP::HMAC<CryptoPP::SHA1>::DEFAULT_KEYLENGTH>
      hmac_key_;
  std::array<unsigned char, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH>
      prf_key_;
};
}
