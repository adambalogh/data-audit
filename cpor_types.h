#pragma once

#include <array>
#include <vector>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/cryptopp/hmac.h"
#include "audit/third_party/cryptopp/sha.h"
#include "audit/third_party/cryptopp/aes.h"

namespace audit {

typedef unsigned char byte;

struct SecretKeys {
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA512>::DEFAULT_KEYLENGTH> mac_key;
  std::array<byte, CryptoPP::AES::DEFAULT_KEYLENGTH> enc_key;
  std::array<byte, CryptoPP::HMAC<CryptoPP::SHA256>::DEFAULT_KEYLENGTH> prf_key;
};
}
