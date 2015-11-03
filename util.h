#pragma once

#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"

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
  virtual CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) = 0;
  virtual ~RandomNumberGenerator() {}
};

// RandomNumberGenerator implementation using CryptoPP
class CryptoNumberGenerator : public RandomNumberGenerator {
 public:
  CryptoPP::Integer GenerateNumber(const CryptoPP::Integer& max) override {
    return CryptoPP::Integer{g, 0, max};
  }

 private:
  CryptoPP::AutoSeededRandomPool g;
};
}
