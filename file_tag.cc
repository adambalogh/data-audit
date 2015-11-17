#include "audit/file_tag.h"

namespace audit {

proto::PrivateFileTag FileTag::PrivateProto() const {
  proto::PrivateFileTag tag;
  for (auto& alpha : alphas) {
    tag.add_alphas(BignumToString(*alpha));
  }
  // TODO add keys
  *tag.mutable_public_tag() = std::move(PublicProto());
  return tag;
}

proto::PublicFileTag FileTag::PublicProto() const {
  proto::PublicFileTag file_tag;
  file_tag.set_num_sectors(num_sectors);
  file_tag.set_num_blocks(num_blocks);
  file_tag.set_sector_size(sector_size);
  BignumToString(*p, file_tag.mutable_p());
  return file_tag;
}
}
