#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"

namespace audit {

class PRF {
 public:
  virtual CryptoPP::Integer Encode(unsigned int i) = 0;
  virtual ~PRF() {}
};

class SiphashPRF : public PRF {
 public:
  SiphashPRF(const unsigned char* key, int length) : hmac_(key, length) {}
  SiphashPRF(const std::string& key)
      : hmac_((unsigned char*)key.data(), key.size()) {}

  CryptoPP::Integer Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
