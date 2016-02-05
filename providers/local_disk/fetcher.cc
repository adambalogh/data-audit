#include "audit/providers/local_disk/fetcher.h"

#include <sstream>

#include "audit/block_tag_map.h"
#include "audit/providers/local_disk/storage.h"

namespace audit {
namespace providers {
namespace local_disk {

Fetcher::Fetcher(const proto::PublicFileTag& file_tag)
    : server::Fetcher(file_tag), block_tag_map_(file_tag.block_tag_map()) {
  file_.open(Storage::GetFilePath(file_tag.file_name()), std::ifstream::binary);
  if (!file_) {
    throw std::runtime_error("Could not open file (" +
                             Storage::GetFilePath(file_tag.file_name()) + ")");
  }

  block_tag_file_.open(Storage::GetBlockTagFilePath(file_tag.file_name()),
                       std::ifstream::binary);
  if (!block_tag_file_) {
    throw std::runtime_error(
        "Could not open file containing BlockTags (" +
        Storage::GetBlockTagFilePath(file_tag.file_name()) + ")");
  }
}

// TODO Check if index is not out of range
std::unique_ptr<std::basic_istream<char>> Fetcher::FetchBlock(
    unsigned long index) {
  auto block_size = file_tag_.num_sectors() * file_tag_.sector_size();
  file_.seekg(index * block_size);

  std::vector<unsigned char> binary(block_size);
  file_.read((char*)binary.data(), binary.size());

  std::string str(binary.begin(), binary.begin() + file_.gcount());
  return std::unique_ptr<std::basic_istream<char>>{
      new std::stringstream{std::move(str)}};
}

proto::BlockTag Fetcher::FetchBlockTag(unsigned long index) {
  size_t start;
  size_t end;
  std::tie(start, end) = block_tag_map_.FindBlockTag(index);

  block_tag_file_.seekg(start);
  std::vector<unsigned char> buffer(end - start);
  block_tag_file_.read((char*)buffer.data(), buffer.size());

  proto::BlockTag tag;
  tag.ParseFromArray(buffer.data(), buffer.size());
  return tag;
}
}
}
}
