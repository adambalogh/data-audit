#include <fstream>
#include <string>

#include "audit/block_tag_map.h"
#include "audit/server/fetcher.h"

namespace audit {

// Fetches from local disk
class LocalDiskFetcher : public Fetcher {
 public:
  LocalDiskFetcher(const proto::PublicFileTag& file_tag);

  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  const proto::PublicFileTag file_tag_;

  const BlockTagMap block_tag_map_;

  std::ifstream file_;
  std::ifstream block_tag_file_;
};
}
