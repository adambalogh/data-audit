#include <fstream>
#include <string>

#include "audit/server/fetcher.h"
#include "audit/client/upload/local_disk_storage.h"

namespace audit {

// Fetches from local disk
class LocalDiskFetcher : public Fetcher {
 public:
  LocalDiskFetcher(const proto::PublicFileTag& file_tag) : file_tag_(file_tag) {
    file_.open(file_dir_ + file_tag_.file_full_path(), std::ifstream::binary);
  }

  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

 private:
  proto::PublicFileTag file_tag_;

  const std::string file_dir_ = upload::LocalDiskStorage::files_dir;

  std::ifstream file_;
};
}
