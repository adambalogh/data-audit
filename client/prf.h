#pragma once

#include <array>
#include <string>

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"

namespace audit {

// PRF is a cryptographic algorithm that encodes an unsigned integer using a
// secret key.
class PRF {
 public:
  virtual BN_ptr Encode(unsigned int i) = 0;

  virtual ~PRF() {}
};

// HMAC-SHA1 based PRF implementation
class HMACPRF : public PRF {
 public:
  typedef std::array<unsigned char,
                     CryptoPP::HMAC<CryptoPP::SHA1>::DEFAULT_KEYLENGTH> KeyType;

  HMACPRF(KeyType key) : key_(key), hmac_{&key[0], key.size()} {}

  BN_ptr Encode(unsigned int i);

 private:
  // Hash-based MAC using SHA-1
  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;

  // Secret key for hmac
  KeyType key_;
};
}
