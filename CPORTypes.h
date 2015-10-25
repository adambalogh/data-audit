#pragma once

#include <vector>

#include "Project/third_party/cryptopp/integer.h"
#include "Project/third_party/cryptopp/hmac.h"
#include "Project/third_party/cryptopp/sha.h"

struct BlockTag {
  long index_;
  CryptoPP::Integer sigma_;
};

struct FileTag {
  long num_blocks_;
  long num_sectors_;
  std::vector<CryptoPP::Integer> alphas_;
};

template <typename MacHashFunction>
struct SecretKey {
  byte mac_key_[CryptoPP::HMAC<MacHashFunction>::DEFAULT_KEYLENGTH];
};
