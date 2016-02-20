#include "audit/providers/local_disk/fetcher.h"

#include <sstream>

#include "audit/server/block_tag_map.h"
#include "audit/client/upload/storage.h"
#include "audit/providers/local_disk/file_storage.h"

using audit::upload::Storage;

namespace audit {
namespace providers {
namespace local_disk {

Fetcher::Fetcher(const proto::PublicFileTag& file_tag)
    : server::Fetcher(file_tag),
      file_name_(file_tag.file_name()),
      block_tag_map_(file_tag.block_tag_map()),
      block_size_(file_tag_.num_sectors() * file_tag_.sector_size()),
      block_binary_(block_size_, 'a'),
      block_tag_binary_(block_tag_map_.MaxSize(), 'a') {}

void Fetcher::Setup() {
  file_.open(FileStorage::dir + Storage::GetFilePath(file_name_),
             std::ifstream::binary);
  if (!file_) {
    throw std::runtime_error("Could not open file (" +
                             Storage::GetFilePath(file_name_) + ")");
  }

  block_tag_file_.open(
      FileStorage::dir + Storage::GetBlockTagFilePath(file_name_),
      std::ifstream::binary);
  if (!block_tag_file_) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             Storage::GetBlockTagFilePath(file_name_) + ")");
  }
}

// TODO Check if index is not out of range
std::unique_ptr<std::basic_istream<char>> Fetcher::FetchBlock(
    unsigned long index) {
  file_.seekg(index * block_size_);
  file_.read(&block_binary_[0], block_binary_.size());

  if (file_.gcount() != block_size_) {
    std::string str(block_binary_.begin(),
                    block_binary_.begin() + file_.gcount());
    return std::unique_ptr<std::basic_istream<char>>{
        new std::stringstream{std::move(str)}};
  }
  return std::unique_ptr<std::basic_istream<char>>{
      new std::stringstream{block_binary_}};
}

proto::BlockTag Fetcher::FetchBlockTag(unsigned long index) {
  size_t start;
  size_t end;
  std::tie(start, end) = block_tag_map_.FindBlockTag(index);
  auto size = end - start;
  assert(size <= block_tag_binary_.size());

  block_tag_file_.seekg(start);
  block_tag_file_.read(&block_tag_binary_[0], size);

  proto::BlockTag tag;
  tag.ParseFromArray(&block_tag_binary_[0], size);
  return std::move(tag);
}
}
}
}
