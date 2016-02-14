#include "audit/client/upload/client.h"

#include <iostream>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/block_tag_serializer.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

Stats Client::Upload(const File& file, ProgressBar::CallbackType callback) {
  // These 2 parameters seem to produce the best results
  TaggingParameters params{30, 96};

  BN_ptr p{BN_new(), ::BN_free};
  BN_generate_prime_ex(p.get(), params.sector_size * 8, false, NULL, NULL,
                       NULL);

  CryptoNumberGenerator random_gen;
  std::vector<BN_ptr> alphas;
  for (int i = 0; i < params.num_sectors; ++i) {
    alphas.push_back(random_gen.GenerateNumber(*p));
  }

  FileContext context{file, params, std::move(alphas), std::move(p),
                      std::unique_ptr<PRF>(new PrfType)};
  auto private_tag = context.Proto();

  size_t total_size = file.size;
  total_size += private_tag.ByteSize();
  total_size += context.num_blocks() * context.parameters().sector_size;
  total_size += context.num_blocks() * context.parameters().sector_size;

  ProgressBar progress_bar{total_size, callback};
  ProgressBarListener progress_listener{progress_bar};

  BlockTagger tagger{context};
  BlockTagSerializer serializer{file.file_name, progress_bar};

  Stats stats;
  stats.file_size = file.size;

  while (tagger.HasNext()) {
    auto tag = tagger.GetNext();
    serializer.Add(tag);
    stats.block_tags_size += tag.ByteSize();
  }

  *private_tag.mutable_public_tag()->mutable_block_tag_map() =
      serializer.Done();
  stats.file_tag_size = private_tag.ByteSize();

  storage_->StoreBlockTagFile(file.file_name, serializer.FileName(),
                              progress_listener);
  storage_->StoreFileTag(file.file_name, private_tag, progress_listener);
  storage_->StoreFile(file.file_name, file.stream, progress_listener);

  serializer.DeleteTempFile();

  return stats;
}
}
}
