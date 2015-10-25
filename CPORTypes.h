#pragma once

#include <vector>

#include "Project/third_party/cryptopp/integer.h"
#include "Project/third_party/cryptopp/hmac.h"
#include "Project/third_party/cryptopp/sha.h"

struct BlockTag {
  long index;
  CryptoPP::Integer sigma;
};

struct FileTag {
  long num_blocks;
  long num_sectors;
  std::vector<CryptoPP::Integer> alphas;
};

template <typename MacHashFunction>
struct SecretKey {
  byte mac_key[CryptoPP::HMAC<MacHashFunction>::DEFAULT_KEYLENGTH];
};
