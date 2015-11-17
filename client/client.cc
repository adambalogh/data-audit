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
  audit::FileTag f{10, 128, std::move(p), &g};

  std::ifstream file{
      "/users/adambalogh/Developer/audit/files_dir/drunkcab.jpg"};

  std::unique_ptr<audit::PRF> prf{new audit::SiphashPRF{"helloka"}};
  audit::BlockTagger block_tagger{file, &f, std::move(prf)};

  int index = 0;
  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
    std::ofstream out{
        "/users/adambalogh/Developer/audit/files_dir/tagsdrunkcab.jpg" +
        std::to_string(index++)};
    tag.SerializeToOstream(&out);
  }
  std::cout << f.num_blocks << std::endl;
}
