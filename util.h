#pragma once

#include <string>

#include "openssl/bn.h"

#include "audit/common.h"

namespace audit {

inline void BignumToString(const BIGNUM& number, std::string* out) {
  out->resize(BN_num_bytes(&number));
  BN_bn2bin(&number, (unsigned char*)out->data());
}

inline std::string BignumToString(const BIGNUM& number) {
  std::string out;
  BignumToString(number, &out);
  return out;
}

inline void StringToBignum(const std::string& in, BIGNUM* out) {
  BN_bin2bn((unsigned char*)in.data(), in.size(), out);
}

inline BN_ptr StringToBignum(const std::string& in) {
  BN_ptr out{BN_new(), ::BN_free};
  StringToBignum(in, out.get());
  return std::move(out);
}

// A generic interface for random number generation
class RandomNumberGenerator {
 public:
  // Returns a random number x, where x >= 0 and x < max
  virtual BN_ptr GenerateNumber(const BIGNUM& max) = 0;
  virtual ~RandomNumberGenerator() {}
};

// RandomNumberGenerator implementation using CryptoPP
class CryptoNumberGenerator : public RandomNumberGenerator {
 public:
  CryptoNumberGenerator();
  BN_ptr GenerateNumber(const BIGNUM& max) override;
};
}
