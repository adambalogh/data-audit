#include "audit/client/file_tag.h"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"
#include "audit/util.h"

namespace audit {

FileTag::FileTag(std::istream& file, const std::string& file_name,
                 unsigned long num_sectors, size_t sector_size,
                 std::vector<BN_ptr> alphas, BN_ptr p)
    : file_(file),
      file_name_(file_name),
      num_sectors_(num_sectors),
      sector_size_(sector_size),
      alphas_(std::move(alphas)),
      p_(std::move(p)) {
  CalculateNumBlocks();
}

void FileTag::CalculateNumBlocks() {
  file_.seekg(0, file_.end);
  auto length = file_.tellg();
  file_.seekg(0, file_.beg);
  auto block_size = sector_size_ * num_sectors_;
  num_blocks_ = length / block_size;
  if (length % block_size != 0) {
    ++num_blocks_;
  }
}

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
  file_tag.set_file_name(file_name_);
  BignumToString(*p_, file_tag.mutable_p());
  return file_tag;
}
}
