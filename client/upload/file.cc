#include "audit/client/upload/file.h"

#include <iostream>

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"
#include "audit/util.h"

namespace audit {
namespace upload {

size_t CalculateSize(std::istream& stream) {
  if (!stream) {
    throw std::runtime_error("The given stream is not open.");
  }
  stream.seekg(0, stream.end);
  auto length = stream.tellg();
  stream.seekg(0, stream.beg);
  return length;
}

File::File(std::unique_ptr<std::istream> s, const std::string& file_name)
    : stream(std::move(s)), file_name(file_name), size(CalculateSize(*stream)) {
  if (!(*stream)) {
    throw std::runtime_error("The given stream is not open.");
  }
}

FileContext::FileContext(File& file, const TaggingParameters& parameters,
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
  auto file_size = file_.size;
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

  auto public_tag = private_tag.mutable_public_tag();
  public_tag->set_num_sectors(parameters_.num_sectors);
  public_tag->set_sector_size(parameters_.sector_size);
  public_tag->set_num_blocks(num_blocks_);
  public_tag->set_file_size(file_.size);
  *public_tag->mutable_p() = BignumToString(*p_);
  *public_tag->mutable_file_name() = file_.file_name;

  return private_tag;
}
}
}
