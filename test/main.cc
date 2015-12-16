#include <sstream>

#include "audit/util.h"
#include "audit/client/tagger.h"
#include "audit/client/local_disk_storage.h"
#include "audit/client/prf.h"

using namespace audit;

int main() {
  Tagger tagger{
      std::unique_ptr<Storage>{new LocalDiskStorage{"test"}},
      std::unique_ptr<RandomNumberGenerator>{new CryptoNumberGenerator},
      std::unique_ptr<PRF>{new HMACPRF{"code"}}};

  std::stringstream file{
      "aejfwoigjqogijwer;goit43io;h5w3[94thg39wa;wighe;oiw4h3;"
      "toihtq09rhgwqpg84538h"};

  tagger.Run(file, "test");
}
