#include "block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "audit/third_party/cryptopp/integer.h"

#include "cpor_types.h"
#include "proto/cpor.pb.h"
#include "prf.h"
#include "util.h"

namespace audit {

proto::BlockTag BlockTagger::GenerateTag() {
  proto::BlockTag tag;
  tag.set_index(file_tag_->num_blocks++);

  auto sigma = CryptoPP::Integer::Zero();
  std::vector<byte> chunk(file_tag_->sector_size);

  for (unsigned int i = 0; i < file_tag_->num_sectors; i++) {
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      assert(i > 0);
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};

    sigma += sector * file_tag_->alphas[i];
    sigma += prf_->Encode(i);
  }

  CryptoIntegerToString(sigma, tag.mutable_sigma());

  return tag;
}

void BlockTagger::CheckValid() {
  if (file_.peek() == std::char_traits<char>::eof()) {
    valid_ = false;
  }
}

proto::BlockTag BlockTagger::GetNext() {
  if (valid_) {
    auto tag = GenerateTag();
    CheckValid();
    return tag;
  }
  return proto::BlockTag{};
}

bool BlockTagger::HasNext() const { return valid_; }
}
