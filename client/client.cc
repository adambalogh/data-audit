#include <iostream>
#include <fstream>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "block_tagger.h"
#include "prf.h"

#include "audit/file_tag.h"
#include "audit/util.h"

int main(int argc, char** argv) {
  audit::CryptoNumberGenerator g;
  audit::BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 11434324234);
  std::ifstream file{argv[1], std::ifstream::binary};
  audit::FileTag f{file, 2000, 16, std::move(p), &g};

  std::unique_ptr<audit::PRF> prf{new audit::HMACPRF{"helloka"}};
  audit::BlockTagger block_tagger{f, std::move(prf)};

  int index = 0;
  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
  }
  std::cout << f.num_blocks() << std::endl;
}
