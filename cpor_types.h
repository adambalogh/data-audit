#pragma once

#include <array>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/hmac.h"
#include "audit/third_party/cryptopp/sha.h"
#include "audit/third_party/cryptopp/aes.h"

namespace audit {

typedef unsigned char byte;

struct BlockTag {
  BlockTag(unsigned long i) : index(i) {}
  BlockTag(unsigned long i, CryptoPP::Integer s) : index(i), sigma(s) {}

  unsigned long index;
  CryptoPP::Integer sigma;
};

struct FileTag {
  unsigned long num_blocks;
  unsigned long num_sectors;
  int sector_size;
  CryptoPP::Integer p;
  std::vector<CryptoPP::Integer> alphas;
};

template <typename HashFunction = CryptoPP::SHA512>
struct SecretKey {
  std::array<byte, CryptoPP::HMAC<HashFunction>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
};

struct Proof {
  CryptoPP::Integer sigma;
  std::vector<CryptoPP::Integer> mus;
};

struct C {
  unsigned long index;
  CryptoPP::Integer weight;
};

struct Challenge {
  std::vector<C> challenges;
};
}
