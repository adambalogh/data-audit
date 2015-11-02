#pragma once

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/siphashxx/siphash.hpp"

namespace audit {

class PRF {
 public:
  virtual CryptoPP::Integer Encode(int i) = 0;
  virtual ~PRF() {}
};

class SiphashPRF : public PRF {
 public:
  SiphashPRF() {}
  CryptoPP::Integer Encode(int i);

 private:
  siphash::stream24_t s_;
};
}
