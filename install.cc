#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"

#include "audit/settings.h"

using namespace CryptoPP;
using namespace audit;

void SetupCrypto(Settings& s) {
  AutoSeededRandomPool rnd;
  SecByteBlock key{AES::DEFAULT_KEYLENGTH};
  rnd.GenerateBlock(key, key.size());
  unsigned char iv[AES::BLOCKSIZE];
  rnd.GenerateBlock(iv, AES::BLOCKSIZE);

  s.Set("key",
        std::string{reinterpret_cast<const char*>(key.data()), key.size()});
  s.Set("iv", std::string{reinterpret_cast<const char*>(&iv), AES::BLOCKSIZE});
}

int main() {
  Settings s(false);
  SetupCrypto(s);
  s.Set("provider", "local_disk");
}
