#pragma once

#include "openssl/bn.h"

namespace audit {

typedef unsigned char byte;

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

BN_ptr BN_ptr_new() {
  BN_ptr n{BN_new(), ::BN_free};
  return std::move(n);
}

BN_ptr BN_ptr_new(unsigned int i) {
  BN_ptr n{BN_new(), ::BN_free};
  BN_set_word(n.get(), i);
  return std::move(n);
}
}
