#include "audit/server/local_disk_fetcher.h"

#include <sstream>

namespace audit {

// TODO Check if index is not out of range
std::basic_istream<char, std::char_traits<char>>& LocalDiskFetcher::FetchBlock(
    unsigned long index) {
  file_.seekg(index * (file_tag_.num_sectors() * file_tag_.sector_size()));
  return file_;
}

proto::BlockTag LocalDiskFetcher::FetchBlockTag(unsigned long index) {
  proto::BlockTag tag;
  std::ifstream tag_file_;
  tag_file_.open(file_dir_ + "tags" + file_tag_.file_name() +
                 std::to_string(index));
  std::stringstream buffer;
  buffer << tag_file_.rdbuf();
  tag.ParseFromString(buffer.str());
  return tag;
}
}
