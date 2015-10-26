#pragma once

#include <array>
#include <vector>

#include "auditing/third_party/cryptopp/integer.h"
#include "auditing/third_party/cryptopp/hmac.h"
#include "auditing/third_party/cryptopp/sha.h"
#include "auditing/third_party/cryptopp/aes.h"

namespace audit {

struct BlockTag {
  long index;
  CryptoPP::Integer sigma;
};

struct FileTag {
  long num_blocks;
  long num_sectors;
  std::vector<CryptoPP::Integer> alphas;
};

template <typename HashFunction = CryptoPP::SHA512>
struct SecretKey {
  std::array<byte, CryptoPP::HMAC<HashFunction>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
};
}
