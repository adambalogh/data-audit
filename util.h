#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "openssl/bn.h"
#include <openssl/rand.h>

#include "common.h"

namespace audit {

inline void CryptoIntegerToString(const CryptoPP::Integer& number,
                                  std::string* out) {
  out->resize(number.MinEncodedSize());
  number.Encode((unsigned char*)out->data(), out->size());
}

inline std::string CryptoIntegerToString(const CryptoPP::Integer& number) {
  std::string out;
  CryptoIntegerToString(number, &out);
  return out;
}

inline void StringToCryptoInteger(const std::string& in,
                                  CryptoPP::Integer* out) {
  out->Decode((unsigned char*)in.data(), in.size());
}

inline CryptoPP::Integer StringToCryptoInteger(const std::string& in) {
  CryptoPP::Integer out;
  StringToCryptoInteger(in, &out);
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
