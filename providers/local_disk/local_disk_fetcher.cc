#include "audit/providers/local_disk/local_disk_fetcher.h"

#include <sstream>

#include "audit/block_tag_map.h"
#include "audit/providers/local_disk/local_disk_storage.h"

namespace audit {

LocalDiskFetcher::LocalDiskFetcher(const proto::PublicFileTag& file_tag)
    : file_tag_(file_tag), block_tag_map_(file_tag.block_tag_map()) {
  file_.open(upload::LocalDiskStorage::GetFilePath(file_tag.file_name()),
             std::ifstream::binary);
  if (!file_) {
    throw std::runtime_error(
        "Could not open file (" +
        upload::LocalDiskStorage::GetFilePath(file_tag.file_name()) + ")");
  }

  block_tag_file_.open(
      upload::LocalDiskStorage::GetBlockTagFilePath(file_tag.file_name()),
      std::ifstream::binary);
  if (!block_tag_file_) {
    throw std::runtime_error(
        "Could not open file containing BlockTags (" +
        upload::LocalDiskStorage::GetBlockTagFilePath(file_tag.file_name()) +
        ")");
  }
}

// TODO Check if index is not out of range
std::basic_istream<char, std::char_traits<char>>& LocalDiskFetcher::FetchBlock(
    unsigned long index) {
  file_.seekg(index * (file_tag_.num_sectors() * file_tag_.sector_size()));
  return file_;
}

// TODO this is kind of slow because of the several seeks
proto::BlockTag LocalDiskFetcher::FetchBlockTag(unsigned long index) {
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
