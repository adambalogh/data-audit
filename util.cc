#include "audit/util.h"

#include <string>

#include "cryptopp/osrng.h"
#include "openssl/bn.h"
#include "openssl/rand.h"

#include "audit/common.h"

namespace audit {

BN_ptr CryptoNumberGenerator::GenerateNumber(const BIGNUM& max) {
  BN_ptr number{BN_new(), ::BN_free};
  BN_rand_range(number.get(), &max);
  return std::move(number);
}
}
