#include <iostream>
#include <fstream>

#include "cryptopp/integer.h"

#include "block_tagger.h"
#include "cpor_types.h"
#include "prf.h"
#include "util.h"

int main(int argc, char** argv) {
  audit::CryptoNumberGenerator g;
  audit::FileTag f{2000, 16, CryptoPP::Integer{9912324324327}, &g};

  std::ifstream file{argv[1]};

  std::unique_ptr<audit::PRF> prf{new audit::SiphashPRF{"helloka"}};
  audit::BlockTagger block_tagger{file, &f, std::move(prf)};

  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
  }
  std::cout << f.num_blocks << std::endl;
}
