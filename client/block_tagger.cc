#include "audit/client/block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/file_tag.h"
#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

bool BlockTagger::FillBuffer() {
  assert(!file_read_);
  auto bytes_left = end_ - start_;

  // If there are any unread bytes,
  // move it to the beginning of the buffer
  std::copy(std::begin(buffer) + start_, std::begin(buffer) + end_,
            std::begin(buffer));

  file_tag_.file().read((char*)buffer.data() + bytes_left,
                        buffer.size() - bytes_left);
  start_ = 0;
  end_ = bytes_left + file_tag_.file().gcount();
  if (end_ != buffer.size()) {
    file_read_ = true;
  }
  return end_ > 0;
}

proto::BlockTag BlockTagger::GenerateTag() {
  BN_ptr sigma{BN_new(), ::BN_free};
  auto encoded_index = prf_->Encode(num_blocks_read_);
  // sigma = sigma + encoded_index
  BN_add(sigma.get(), sigma.get(), encoded_index.get());

  BN_ptr sector{BN_new(), ::BN_free};

  // Go through each sector in block
  for (auto i = 0; i < file_tag_.num_sectors(); ++i) {
    if (file_read_ && start_ >= end_) break;
    if (start_ + file_tag_.sector_size() > end_ && !file_read_) {
      FillBuffer();
    }

    BN_bin2bn(buffer.data() + start_,
              std::min(file_tag_.sector_size(), (unsigned long)end_ - start_),
              sector.get());
    // sector = sector * alpha[i]
    BN_mul(sector.get(), file_tag_.alphas().at(i).get(), sector.get(),
           ctx.get());
    // sigma = sigma + sector
    BN_add(sigma.get(), sigma.get(), sector.get());

    start_ += file_tag_.sector_size();
    BN_clear(sector.get());
  }
  // sigma = sigma % p
  BN_mod(sigma.get(), sigma.get(), file_tag_.p(), ctx.get());

  proto::BlockTag tag;
  tag.set_index(num_blocks_read_++);
  BignumToString(*sigma, tag.mutable_sigma());

  return tag;
}

proto::BlockTag BlockTagger::GetNext() { return GenerateTag(); }

bool BlockTagger::HasNext() const {
  return num_blocks_read_ < file_tag_.num_blocks();
}
}
