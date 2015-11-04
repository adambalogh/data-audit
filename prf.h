#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "cpor_types.h"

namespace audit {

class PRF {
 public:
  virtual BN_ptr Encode(unsigned int i) = 0;
  virtual ~PRF() {}
};

class SiphashPRF : public PRF {
 public:
  SiphashPRF(const unsigned char* key, int length) : hmac_(key, length) {}
  SiphashPRF(const std::string& key)
      : hmac_((unsigned char*)key.data(), key.size()) {}

  BN_ptr Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
