#include "file_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "audit/third_party/cryptopp/integer.h"
#include "audit/third_party/siphash/siphash24.h"

#include "cpor_types.h"

namespace audit {

void FileTagger::MakeAlphas() {
  std::generate(std::begin(alphas_), std::end(alphas_),
                [&]() { return random_gen_.GenerateNumber(p_); });
}

BlockTag FileTagger::GenerateTag() {
  BlockTag tag{num_blocks_};
  ++num_blocks_;

  std::vector<byte> chunk(sector_size_);

  for (int i = 0; i < num_sectors_; i++) {
    // TODO Ugly hack
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      assert(i > 0);
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};
    tag.sigma += sector * alphas_[i];
  }

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
  return BlockTag{0};
}

bool FileTagger::HasNext() const { return valid_; }

FileTag FileTagger::GetFileTag() {
  FileTag t;
  t.num_blocks = num_blocks_;
  t.num_sectors = num_sectors_;
  t.sector_size = sector_size_;
  t.p = p_;
  t.alphas = alphas_;

  return t;
}
}
