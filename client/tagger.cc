#include "audit/client/tagger.h"

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/block_tagger.h"
#include "audit/client/file_tag.h"
#include "audit/client/storage.h"

namespace audit {

void Tagger::Run(std::istream& file, const std::string& file_name) {
  unsigned long num_sectors = 10;
  size_t sector_size = 128;

  BN_ptr p{BN_new(), ::BN_free};
  BN_generate_prime_ex(p.get(), sector_size, false, NULL, NULL, NULL);

  std::vector<BN_ptr> alphas;
  for (int i = 0; i < num_sectors; ++i) {
    alphas.push_back(random_gen_->GenerateNumber(*p));
  }

  FileTag file_tag{file, file_name, num_sectors, sector_size, std::move(alphas),
                   std::move(p)};

  BlockTagger tagger{file_tag, std::move(prf_)};
  while (tagger.HasNext()) {
    storage_->StoreBlockTag(tagger.GetNext());
  }

  storage_->StoreFileTag(file_tag.PrivateProto());

  file.clear();
  file.seekg(0, file.beg);
  storage_->StoreFile(file);
}
}
