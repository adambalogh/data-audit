#include "block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "cryptopp/integer.h"

#include "cpor_types.h"
#include "proto/cpor.pb.h"
#include "prf.h"
#include "util.h"

namespace audit {

// TODO optimize disk access
proto::BlockTag BlockTagger::GenerateTag() {
  auto sigma = CryptoPP::Integer::Zero();
  sigma += prf_->Encode(file_tag_->num_blocks);

  std::vector<byte> chunk(file_tag_->sector_size);

  for (unsigned int i = 0; i < file_tag_->num_sectors; ++i) {
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      assert(i > 0);
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};

    sigma += sector * file_tag_->alphas[i];
  }
  sigma = sigma.Modulo(file_tag_->p);

  proto::BlockTag tag;
  tag.set_index(file_tag_->num_blocks++);
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
