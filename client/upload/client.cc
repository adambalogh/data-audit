#include "audit/client/upload/client.h"

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

Stats Client::Upload(const File& file) {
  TaggingParameters params{10, 128};

  BN_ptr p{BN_new(), ::BN_free};
  BN_generate_prime_ex(p.get(), params.sector_size * 8, false, NULL, NULL,
                       NULL);

  CryptoNumberGenerator random_gen;
  std::vector<BN_ptr> alphas;
  for (int i = 0; i < params.num_sectors; ++i) {
    alphas.push_back(random_gen.GenerateNumber(*p));
  }

  FileContext context{file, params, std::move(alphas), std::move(p),
                      std::unique_ptr<PRF>(new HMACPRF)};

  StorageWithStats unique_storage{storage_.get()};

  BlockTagger tagger{context};
  while (tagger.HasNext()) {
    // TODO this is a very inefficient way of doing it, especially if it's
    // stored on AWS or something similar
    unique_storage.StoreBlockTag(file, tagger.GetNext());
  }

  unique_storage.StoreFileTag(file, context.Proto());
  unique_storage.StoreFile(file);

  return unique_storage.GetStats();
}
}
}
