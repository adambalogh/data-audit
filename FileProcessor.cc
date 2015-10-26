#include "FileProcessor.h"

#include <iostream>
#include <string>

#include "CPORTypes.h"

namespace audit {

BlockTag FileProcessor::GenerateTag() {
  BlockTag tag{0};
  std::vector<byte> chunk(sector_size_);

  for (int i = 0; i < num_sectors_; i++) {
    // TODO Ugly hack
    file_.read((char*)chunk.data(), chunk.size());
    size_t bytes_read = file_.gcount();

    if (!bytes_read) {
      break;
    }

    CryptoPP::Integer sector{chunk.data(), bytes_read};
    tag.sigma += sector * 1;
  }

  return tag;
}
}
