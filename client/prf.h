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

  HMACPRF()
      : key_([]() {
          KeyType key;
          BN_ptr prf_key{BN_new(), ::BN_free};
          BN_rand(prf_key.get(), key.size() * 8, 0, 1);
          BN_bn2bin(prf_key.get(), &key[0]);
          return std::move(key);
        }()),
        hmac_(&key_[0], key_.size()) {}

  BN_ptr Encode(unsigned int i);

 private:
  // Secret key for hmac
  KeyType key_;

  CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
