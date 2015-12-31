#include "audit/client/block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/file.h"
#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

void BlockTagger::FillBuffer() {
  assert(!file_read_);
  auto bytes_left = end_ - start_;

  // If there are any unread bytes,
  // move it to the beginning of the buffer
  std::copy(std::begin(buffer) + start_, std::begin(buffer) + end_,
            std::begin(buffer));

  context_.file().stream.read((char*)buffer.data() + bytes_left,
                              buffer.size() - bytes_left);
  start_ = 0;
  end_ = bytes_left + context_.file().stream.gcount();
  if (end_ != buffer.size()) {
    file_read_ = true;
  }
}

/* The formula for calculating a Tag's sigma is as follows:
 *
 * sigma = 0
 * sigma += PRF(tag_index)
 * for (int i = 0; i < num_sectors; ++i) {
 *   sigma += sectors[i] * alphas[i]
 * }
 * sigma = sigma % p
 *
 */
proto::BlockTag BlockTagger::GenerateTag() {
  BN_ptr sigma{BN_new(), ::BN_free};
  auto encoded_index = context_.prf().Encode(num_blocks_read_);
  BN_add(sigma.get(), sigma.get(), encoded_index.get());

  BN_ptr sector{BN_new(), ::BN_free};

  for (auto i = 0; i < context_.parameters().num_sectors; ++i) {
    if (file_read_ && start_ >= end_) break;
    if (start_ + context_.parameters().sector_size > end_ && !file_read_) {
      FillBuffer();
    }
    BN_bin2bn(buffer.data() + start_,
              std::min(context_.parameters().sector_size,
                       (unsigned long)end_ - start_),
              sector.get());
    BN_mul(sector.get(), context_.alphas().at(i).get(), sector.get(),
           ctx.get());
    BN_add(sigma.get(), sigma.get(), sector.get());

    start_ += context_.parameters().sector_size;
    BN_clear(sector.get());
  }
  BN_mod(sigma.get(), sigma.get(), context_.p(), ctx.get());

  proto::BlockTag tag;
  tag.set_index(num_blocks_read_++);
  BignumToString(*sigma, tag.mutable_sigma());

  return tag;
}

proto::BlockTag BlockTagger::GetNext() { return GenerateTag(); }

bool BlockTagger::HasNext() const {
  return num_blocks_read_ < context_.num_blocks();
}
}
