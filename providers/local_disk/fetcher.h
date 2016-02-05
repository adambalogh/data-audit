#include <fstream>
#include <string>

#include "audit/block_tag_map.h"
#include "audit/server/fetcher.h"

namespace audit {
namespace providers {
namespace local_disk {

// Fetches from local disk
class Fetcher : public server::Fetcher {
 public:
  Fetcher(const proto::PublicFileTag& file_tag);

  std::unique_ptr<std::basic_istream<char>> FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  const BlockTagMap block_tag_map_;

  std::ifstream file_;
  std::ifstream block_tag_file_;
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
