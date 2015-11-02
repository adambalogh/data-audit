#include "file_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "audit/third_party/cryptopp/integer.h"

#include "proto/cpor.pb.h"
#include "prf.h"
#include "util.h"

namespace audit {

BlockTag FileTagger::GenerateTag() {
  BlockTag tag;
  auto sigma = CryptoPP::Integer::Zero();
  std::vector<byte> chunk(file_tag_->sector_size());

  for (unsigned int i = 0; i < file_tag_->num_sectors(); i++) {
    // TODO Ugly hack
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      assert(i > 0);
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};

    sigma += sector * file_tag_->alphas()[i];
    sigma += prf_->Encode(i);
  }

  // TODO fix it
  tag.set_index(0);
  CryptoIntegerToString(sigma, tag.mutable_sigma());

  return tag;
}

void FileTagger::CheckValid() {
  if (file_.peek() == std::char_traits<char>::eof()) {
    valid_ = false;
  }
}

BlockTag FileTagger::GetNext() {
  if (valid_) {
    auto tag = GenerateTag();
    CheckValid();
    return tag;
  }
  return BlockTag{};
}

bool FileTagger::HasNext() const { return valid_; }
}
