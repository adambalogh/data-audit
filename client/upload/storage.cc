#include "audit/client/upload/storage.h"

#include <fstream>
#include <sstream>

namespace audit {
namespace upload {

void Storage::StoreFile(const std::string& file_name, std::istream& stream,
                        StorageListener& listener) {
  if (!stream) {
    throw std::runtime_error{"Given stream for file '" + file_name +
                             "' is not open"};
  }
  file_storage_->StoreFile(GetFilePath(file_name), stream, listener);
}

void Storage::StoreFileTag(const std::string& file_name,
                           const proto::PrivateFileTag& file_tag,
                           StorageListener& listener) {
  std::stringstream stream{file_tag.SerializeAsString()};
  file_storage_->StoreFile(GetFileTagPath(file_name), stream, listener);
}

void Storage::StoreBlockTagFile(const std::string& file_name,
                                const std::string& block_file_path,
                                StorageListener& listener) {
  std::ifstream block_tag_file{block_file_path, std::ifstream::binary};
  if (!block_tag_file) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }
  file_storage_->StoreFile(GetBlockTagFilePath(file_name), block_tag_file,
                           listener);
}
}
}
