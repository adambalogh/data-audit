#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/file_tag.h"

namespace audit {

class PRF {
 public:
  virtual BN_ptr Encode(unsigned int i) = 0;
  virtual ~PRF() {}
};

class HMACPRF : public PRF {
 public:
  HMACPRF(const unsigned char* key, int length) : hmac_(key, length) {}
  HMACPRF(const std::string& key)
      : hmac_((unsigned char*)key.data(), key.size()) {}

  BN_ptr Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
