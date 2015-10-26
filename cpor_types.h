#pragma once

#include <array>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/hmac.h"
#include "audit/third_party/cryptopp/sha.h"
#include "audit/third_party/cryptopp/aes.h"

namespace audit {

typedef unsigned char byte;

struct Block {};

struct BlockTag {
  BlockTag(long i) : index(i) {}
  BlockTag(long i, CryptoPP::Integer s) : index(i), sigma(s) {}

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

struct Challenge {};

struct CPORContext {
  CryptoPP::Integer p;
};
}
