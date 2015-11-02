#pragma once

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/hmac.h"
#include "audit/third_party/cryptopp/sha.h"

namespace audit {

class PRF {
 public:
  virtual CryptoPP::Integer Encode(unsigned int i) = 0;
  virtual ~PRF() {}
};

class SiphashPRF : public PRF {
 public:
  SiphashPRF(const unsigned char* key, int length) : hmac_(key, length) {}
  CryptoPP::Integer Encode(unsigned int i);

 private:
  CryptoPP::HMAC<CryptoPP::SHA256> hmac_;
};
}
