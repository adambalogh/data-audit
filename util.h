#include <string>
#include "audit/third_party/cryptopp/integer.h"

namespace audit {

inline void CryptoIntegerToString(const CryptoPP::Integer& number,
                                  std::string* out) {
  out->reserve(number.MinEncodedSize());
  number.Encode((unsigned char*)out->data(), out->size());
}

inline void StringToCryptoInteger(const std::string& in,
                                  CryptoPP::Integer* out) {
  out->Decode((unsigned char*)in.data(), in.size());
}
}
