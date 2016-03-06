#include "audit/client/upload/client.h"

#include <iostream>
#include <thread>

#include "openssl/bn.h"
#include "lz4.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/block_tag_serializer.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/storage.h"
#include "audit/client/upload/progress_bar.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

std::string CompressedPath(const std::string& file_name) {
  return application_dir + "compressed/" + file_name;
}

void Client::Compress(const File& file) const {
  std::ofstream compressed_out(CompressedPath(file.file_name),
                               std::ofstream::binary);
  if (!compressed_out) {
    throw std::runtime_error("Could not open file to write compressed file");
  }

  std::array<char, 10000> buffer;
  std::vector<char> compressed_buffer(LZ4_compressBound(buffer.size()));
  size_t read;
  size_t compressed_size;
  while (file.stream.read(buffer.data(), buffer.size()).gcount()) {
    read = file.stream.gcount();
    compressed_size =
        LZ4_compress_default(buffer.data(), compressed_buffer.data(), read,
                             compressed_buffer.size());
    compressed_out.write(compressed_buffer.data(), compressed_size);
  }
}

Stats Client::Upload(const File& f, const TaggingParameters& params,
                     ProgressBar::CallbackType callback) {
  BN_ptr p{BN_new(), ::BN_free};
  BN_generate_prime_ex(p.get(), params.sector_size * 8, false, NULL, NULL,
                       NULL);

  CryptoNumberGenerator random_gen;
  std::vector<BN_ptr> alphas;
  for (int i = 0; i < params.num_sectors; ++i) {
    alphas.push_back(random_gen.GenerateNumber(*p));
  }

  // Compress file using LZ4
  Compress(f);
  std::ifstream stream{CompressedPath(f.file_name), std::ifstream::binary};
  if (!stream) {
    throw std::runtime_error("Compressed file could not be opened");
  }
  File compressed_file{stream, f.file_name};

  FileContext context{compressed_file, params, std::move(alphas), std::move(p),
                      std::unique_ptr<PRF>(new PrfType)};
  auto private_tag = context.Proto();

  size_t total_size = compressed_file.size;
  total_size += private_tag.ByteSize();
  total_size += context.num_blocks() * context.parameters().sector_size;
  total_size += context.num_blocks() * context.parameters().sector_size;

  ProgressBar progress_bar{total_size, callback};
  ProgressBarListener progress_listener{progress_bar};

  BlockTagger tagger{context};
  BlockTagSerializer serializer{compressed_file.file_name, progress_bar};

  Stats stats;
  stats.file_size = compressed_file.size;

  while (tagger.HasNext()) {
    auto tag = tagger.GetNext();
    serializer.Add(tag);
    stats.block_tags_size += tag.ByteSize();
  }

  *private_tag.mutable_public_tag()->mutable_block_tag_map() =
      serializer.Done();
  stats.file_tag_size = private_tag.ByteSize();

  std::thread t1{[&]() {
    storage_.StoreBlockTagFile(compressed_file.file_name, serializer.FilePath(),
                               progress_listener);
  }};
  std::thread t2{[&]() {
    storage_.StoreFileTag(compressed_file.file_name, private_tag,
                          progress_listener);
  }};
  std::thread t3{[&]() {
    storage_.StoreFile(compressed_file.file_name, compressed_file.stream,
                       progress_listener);
  }};

  t1.join();
  t2.join();
  t3.join();

  // Remove temp compressed file
  std::remove(CompressedPath(f.file_name).c_str());

  return stats;
}
}
}
