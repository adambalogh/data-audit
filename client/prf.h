#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"

namespace audit {

// PRF is a cryptographic algorithm that encodes an unsigned integer using a
// secret key.
class PRF {
 public:
  virtual BN_ptr Encode(unsigned int i) = 0;
  virtual void SetKey(const unsigned char* key, size_t size) = 0;

  void SetKey(const std::string& key) {
    SetKey((unsigned char*)key.data(), key.size());
  }

  virtual ~PRF() {}

 protected:
  std::string key_;
};

// HMAC-SHA1 based PRF implementation
class HMACPRF : public PRF {
 public:
  using PRF::SetKey;

  void SetKey(const unsigned char* key, size_t size) {
    hmac_.SetKey(key, size);
  }

  BN_ptr Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_{0, 0};
};
}
