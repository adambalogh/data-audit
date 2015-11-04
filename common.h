#include "openssl/bn.h"

namespace audit {

typedef unsigned char byte;

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
}
