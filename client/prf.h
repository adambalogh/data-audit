#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/file_tag.h"

namespace audit {

// PRF is a cryptographic algorithm that encodes an unsigned integer using a
// secret key.
class PRF {
 public:
  PRF(const unsigned char* key, int length) : key_(key), length_(length) {}
  virtual BN_ptr Encode(unsigned int i) = 0;
  virtual ~PRF() {}

 protected:
  const unsigned char* key_;
  int length_;
};

// HMAC-SHA1 based PRF implementation
class HMACPRF : public PRF {
 public:
  HMACPRF(const unsigned char* key, int length)
      : PRF(key, length), hmac_(key_, length_) {}

  HMACPRF(const std::string& key)
      : HMACPRF((unsigned char*)key.data(), key.size()) {}

  BN_ptr Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
