#pragma once

#include <array>
#include <string>

#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"

namespace audit {

// PRF is a cryptographic algorithm that encodes an unsigned integer using a
// secret key.
class PRF {
 public:
  virtual BN_ptr Encode(unsigned int i) const = 0;

  // Returns the secret key
  virtual std::string Key() const = 0;

  virtual ~PRF() {}
};

// HMAC-SHA1 based PRF implementation
class HMACPRF : public PRF {
 public:
  typedef std::array<unsigned char,
                     CryptoPP::HMAC<CryptoPP::SHA1>::DEFAULT_KEYLENGTH> KeyType;

  HMACPRF();
  HMACPRF(const std::string& key);

  std::string Key() const override;

  BN_ptr Encode(unsigned int i) const override;

 private:
  // Secret key for hmac
  KeyType key_;

  mutable CryptoPP::HMAC<CryptoPP::SHA1> hmac_;
};
}
