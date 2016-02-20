#pragma once

#include <fstream>
#include <string>

#include "audit/server/block_tag_map.h"
#include "audit/server/fetcher.h"

namespace audit {
namespace providers {
namespace local_disk {

// Fetches from local disk
//
class Fetcher : public server::Fetcher {
 public:
  Fetcher(const proto::PublicFileTag& file_tag);

  void Setup() override;

  std::unique_ptr<std::basic_istream<char>> FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 protected:
  const std::string file_name_;

  const BlockTagMap block_tag_map_;

  std::ifstream file_;

  std::ifstream block_tag_file_;

  // The size of a block within this file
  size_t block_size_;

  // Buffer for reading the file
  std::string block_binary_;

  // Buffer for reading the BlockTag file
  std::string block_tag_binary_;
};

class FetcherFactory : public server::FetcherFactory {
 public:
  std::unique_ptr<server::Fetcher> Create(
      const proto::PublicFileTag& file_tag) override {
    return std::unique_ptr<server::Fetcher>{new Fetcher{file_tag}};
  }
};
}
}
}
