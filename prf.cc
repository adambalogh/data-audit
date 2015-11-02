#include "prf.h"

namespace audit {

CryptoPP::Integer SiphashPRF::Encode(int i) {
  unsigned char bytes[4];

  bytes[0] = (i >> 24) & 0xFF;
  bytes[1] = (i >> 16) & 0xFF;
  bytes[2] = (i >> 8) & 0xFF;
  bytes[3] = i & 0xFF;

  s_.reset(prf_key);
  s_.update(4, bytes);
  auto v = s_.finish();

  return CryptoPP::Integer{v};
}
}
