#include <iostream>
#include <fstream>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "block_tagger.h"
#include "cpor_types.h"
#include "prf.h"
#include "util.h"

int main(int argc, char** argv) {
  audit::CryptoNumberGenerator g;
  audit::BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 11434324234);
  audit::FileTag f{1000, 128, std::move(p), &g};

  std::ifstream file{argv[1]};

  std::unique_ptr<audit::PRF> prf{new audit::SiphashPRF{"helloka"}};
  audit::BlockTagger block_tagger{file, &f, std::move(prf)};

  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
  }
  std::cout << f.num_blocks << std::endl;
}
