#include <fstream>
#include <string>

#include "audit/server/fetcher.h"

namespace audit {

// Fetches from local disk
class LocalDiskFetcher : public Fetcher {
 public:
  LocalDiskFetcher(const std::string& file_name,
                   const proto::PublicFileTag& file_tag)
      : file_name_(file_name), file_tag_(file_tag) {
    file_.open("/users/adambalogh/Developer/audit/files_dir/" + file_name);
  }

  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  std::string file_name_;
  proto::PublicFileTag file_tag_;

  std::ifstream file_;
};
}
