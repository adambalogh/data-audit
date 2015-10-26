#include "file_tagger.h"

#include <iostream>
#include <string>

#include "cpor_types.h"

namespace audit {

void FileTagger::MakeAlphas() {
  std::generate(std::begin(alphas_), std::end(alphas_), []() { return 1; });
}

BlockTag FileTagger::GenerateTag() {
  BlockTag tag{0};
  std::vector<byte> chunk(sector_size_);

  for (int i = 0; i < num_sectors_; i++) {
    // TODO Ugly hack
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      valid_ = false;
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};
    tag.sigma += sector * alphas_[i];
  }

  CheckValid();
  return tag;
}

void FileTagger::CheckValid() {
  if (file_.peek() == std::char_traits<char>::eof()) {
    valid_ = false;
    return;
  }
}
}
