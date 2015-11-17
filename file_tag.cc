#include "audit/file_tag.h"

namespace audit {

proto::PrivateFileTag FileTag::PrivateProto() const {
  proto::PrivateFileTag tag;
  for (auto& alpha : alphas_) {
    tag.add_alphas(BignumToString(*alpha));
  }
  // TODO add keys
  *tag.mutable_public_tag() = std::move(PublicProto());
  return tag;
}

proto::PublicFileTag FileTag::PublicProto() const {
  proto::PublicFileTag file_tag;
  file_tag.set_num_sectors(num_sectors_);
  file_tag.set_num_blocks(num_blocks_);
  file_tag.set_sector_size(sector_size_);
  BignumToString(*p_, file_tag.mutable_p());
  return file_tag;
}
}
