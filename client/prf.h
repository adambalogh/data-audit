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
  PRF(const std::string& key) : key_(key) {}
  virtual BN_ptr Encode(unsigned int i) = 0;
  virtual ~PRF() {}

 protected:
  std::string key_;
};

// HMAC-SHA1 based PRF implementation
class HMACPRF : public PRF {
 public:
  HMACPRF(const std::string& key) : PRF(key) {}

  BN_ptr Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_{(unsigned char*)key_.data(),
                                       key_.size()};
};
}
