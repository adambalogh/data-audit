#include "audit/client/file.h"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"
#include "audit/util.h"

namespace audit {

File::File(std::istream& stream, int num_sectors, size_t sector_size,
           std::vector<BN_ptr> alphas, BN_ptr p)
    : stream_(stream),
      num_sectors_(num_sectors),
      sector_size_(sector_size),
      alphas_(std::move(alphas)),
      p_(std::move(p)) {
  if (stream_) {
    throw std::runtime_error("The given file cannot be read from.");
  }
  if (alphas_.size() != num_sectors) {
    throw std::length_error(
        "The size of alphas must be equal to num_sectors (" +
        std::to_string(alphas_.size()) + " != " + std::to_string(num_sectors) +
        ")");
  }
  CalculateNumBlocks();
}

void File::CalculateNumBlocks() {
  stream_.seekg(0, stream_.end);
  auto length = stream_.tellg();
  stream_.seekg(0, stream_.beg);
  auto block_size = sector_size_ * num_sectors_;
  num_blocks_ = length / block_size;
  if (length % block_size != 0) {
    ++num_blocks_;
  }
}
}
