#include "audit/client/client.h"

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/block_tagger.h"
#include "audit/client/file.h"
#include "audit/client/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

void Client::Upload(std::istream& stream, const std::string& file_name) {
  proto::PrivateFileTag file_tag;

  int num_sectors = 10;
  size_t sector_size = 128;

  BN_ptr p{BN_new(), ::BN_free};
  BN_generate_prime_ex(p.get(), sector_size * 8, false, NULL, NULL, NULL);

  std::vector<BN_ptr> alphas;
  for (int i = 0; i < num_sectors; ++i) {
    alphas.push_back(random_gen_->GenerateNumber(*p));
  }

  File file{stream, num_sectors, sector_size, std::move(alphas), std::move(p)};

  BlockTagger tagger{file, std::move(prf_)};
  while (tagger.HasNext()) {
    storage_->StoreBlockTag(tagger.GetNext());
  }

  storage_->StoreFileTag(file_tag);

  stream.clear();
  stream.seekg(0, stream.beg);
  storage_->StoreFile(stream);
}
}
