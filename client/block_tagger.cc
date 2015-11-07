#include "audit/client/block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/cpor_types.h"
#include "audit/proto/cpor.pb.h"
#include "audit/client/prf.h"
#include "audit/client/util.h"

namespace audit {

bool BlockTagger::FillBuffer() {
  assert(!file_read_);
  auto bytes_left = end_ - start_;

  // If there are any unread bytes, move it to the beginning of the buffer
  std::copy(std::begin(buffer) + start_, std::begin(buffer) + end_,
            std::begin(buffer));

  file_.read((char*)buffer.data() + bytes_left, buffer.size() - bytes_left);
  start_ = 0;
  end_ = bytes_left + file_.gcount();
  if (end_ != buffer.size()) {
    file_read_ = true;
  }
  return end_ > 0;
}

proto::BlockTag BlockTagger::GenerateTag() {
  // TODO free this
  BN_CTX* ctx = BN_CTX_new();
  BN_ptr sigma{BN_new(), ::BN_free};
  auto encoded_index = prf_->Encode(num_blocks_read_);

  // sigma = sigma + encoded_index
  BN_add(sigma.get(), sigma.get(), encoded_index.get());

  for (unsigned int i = 0; i < file_tag_->num_sectors; ++i) {
    if (file_read_ && start_ >= end_) break;
    if (start_ + file_tag_->sector_size > end_ && !file_read_) {
      FillBuffer();
    }

    BN_ptr sector{BN_new(), ::BN_free};
    BN_bin2bn(buffer.data() + start_,
              std::min(file_tag_->sector_size,
                       static_cast<unsigned long>(end_ - start_)),
              sector.get());

    // sector = sector * alpha[i]
    BN_mul(sector.get(), file_tag_->alphas[i].get(), sector.get(), ctx);
    // sigma = sigma + sector
    BN_add(sigma.get(), sigma.get(), sector.get());

    start_ += file_tag_->sector_size;
  }
  // sigma = sigma % p
  BN_mod(sigma.get(), sigma.get(), file_tag_->p.get(), ctx);

  proto::BlockTag tag;
  tag.set_index(num_blocks_read_++);
  BignumToString(*sigma, tag.mutable_sigma());

  return tag;
}

proto::BlockTag BlockTagger::GetNext() { return GenerateTag(); }

bool BlockTagger::HasNext() const {
  return num_blocks_read_ < file_tag_->num_blocks;
}
}
