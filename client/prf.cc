#include "audit/client/prf.h"

#include "cryptopp/integer.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "openssl/bn.h"

#include "audit/common.h"

namespace audit {

HMACPRF::HMACPRF()
    : key_([]() {
        KeyType key;
        BN_ptr prf_key{BN_new(), ::BN_free};
        BN_rand(prf_key.get(), key.size() * 8, 0, 1);
        BN_bn2bin(prf_key.get(), &key[0]);
        return std::move(key);
      }()),
      hmac_(&key_[0], key_.size()) {}

HMACPRF::HMACPRF(const std::string& key)
    : key_([&]() {
        if (key.size() != key_.size()) {
          throw std::length_error("The size of the given key is invalid");
        }
        KeyType new_key;
        for (int i = 0; i < key.size(); ++i) {
          new_key[i] = key[i];
        }
        return new_key;
      }()),
      hmac_(&key_[0], key_.size()) {}

BN_ptr HMACPRF::Encode(unsigned int i) const {
  hmac_.Restart();

  // Convert i to byte array
  unsigned char bytes[4];
  bytes[0] = (i >> 24) & 0xFF;
  bytes[1] = (i >> 16) & 0xFF;
  bytes[2] = (i >> 8) & 0xFF;
  bytes[3] = i & 0xFF;

  unsigned char digest[hmac_.DigestSize()];
  hmac_.Update(&bytes[0], 4);
  hmac_.Final(&digest[0]);

  BN_ptr result{BN_new(), ::BN_free};
  BN_bin2bn(&digest[0], hmac_.DigestSize(), result.get());

  return std::move(result);
}

std::string HMACPRF::Key() const {
  return std::string{key_.begin(), key_.end()};
}
}
