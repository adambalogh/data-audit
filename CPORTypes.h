#pragma once

#include <vector>

#include "auditing/third_party/cryptopp/integer.h"
#include "auditing/third_party/cryptopp/hmac.h"
#include "auditing/third_party/cryptopp/sha.h"

struct BlockTag {
  long index;
  CryptoPP::Integer sigma;
};

struct FileTag {
  long num_blocks;
  long num_sectors;
  std::vector<CryptoPP::Integer> alphas;
};

template <typename HashFunction>
struct SecretKey {
  byte mac_key[CryptoPP::HMAC<HashFunction>::DEFAULT_KEYLENGTH];
};
