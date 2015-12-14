#include <fstream>
#include <string>

#include "audit/server/fetcher.h"

namespace audit {

// Fetches from local disk
class LocalDiskFetcher : public Fetcher {
 public:
  LocalDiskFetcher(const proto::PublicFileTag& file_tag,
                   const std::string& path)
      : file_tag_(file_tag), path_(path) {
    file_.open(path_ + file_tag_.file_name(), std::ifstream::binary);
  }

  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  proto::PublicFileTag file_tag_;
  std::string path_;

  std::ifstream file_;
};
}
