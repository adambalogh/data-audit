#include "audit/client/upload/file.h"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"
#include "audit/util.h"

namespace audit {

File::File(std::istream& stream, const std::string& file_full_path)
    : stream(stream), file_full_path(file_full_path) {
  if (!stream) {
    throw std::runtime_error("The given stream cannot be read from.");
  }
}

FileContext::FileContext(const File& file, const TaggingParameters& parameters,
                         std::vector<BN_ptr> alphas, BN_ptr p,
                         std::unique_ptr<PRF> prf)
    : file_(file),
      parameters_(parameters),
      num_blocks_(CalculateNumBlocks()),
      alphas_(std::move(alphas)),
      p_(std::move(p)),
      prf_(std::move(prf)) {
  if (alphas_.size() != parameters_.num_sectors) {
    throw std::length_error(
        "The size of alphas must be equal to num_sectors (" +
        std::to_string(alphas_.size()) + " != " +
        std::to_string(parameters_.num_sectors) + ")");
  }
}

int FileContext::CalculateNumBlocks() {
  auto file_size = file_.size();
  auto block_size = parameters_.block_size();

  int num_blocks = file_size / block_size;
  if (file_size % block_size != 0) {
    ++num_blocks;
  }
  return num_blocks;
}

proto::PrivateFileTag FileContext::Proto() const {
  proto::PrivateFileTag private_tag;

  for (auto& alpha : alphas_) {
    BignumToString(*alpha, private_tag.add_alphas());
  }
  *private_tag.mutable_prf_key() = prf_->Key();

  auto public_tag = private_tag.public_tag();
  public_tag.set_num_sectors(parameters_.num_sectors);
  public_tag.set_sector_size(parameters_.sector_size);
  public_tag.set_num_blocks(num_blocks_);
  BignumToString(*p_, public_tag.mutable_p());
  *public_tag.mutable_file_full_path() = file_.file_full_path;

  return private_tag;
}
}
