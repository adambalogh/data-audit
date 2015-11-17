#include <fstream>
#include <string>

#include "audit/server/fetcher.h"

namespace audit {

// Fetches from local disk
class LocalDiskFetcher : public Fetcher {
 public:
  LocalDiskFetcher(const proto::PublicFileTag& file_tag) : file_tag_(file_tag) {
    file_.open(
        "/users/adambalogh/Developer/audit/files_dir/" + file_tag_.file_name(),
        std::ifstream::binary);
  }

  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  proto::PublicFileTag file_tag_;

  std::ifstream file_;
};
}
