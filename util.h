#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "openssl/bn.h"
#include <openssl/rand.h>

#include "common.h"

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

inline BIGNUM* StringToBignum(const std::string& in) {
  BIGNUM* out = BN_new();
  StringToBignum(in, out);
  return out;
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
  CryptoNumberGenerator() {
    // TODO add better seed
    std::string seed{"seeeeeeeeeeed"};
    RAND_seed(seed.c_str(), seed.size());
  }

  BN_ptr GenerateNumber(const BIGNUM& max) override {
    BN_ptr number{BN_new(), ::BN_free};
    BN_rand_range(number.get(), &max);
    return std::move(number);
  }
};
}
