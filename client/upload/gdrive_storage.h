#pragma once

#include "audit/client/upload/storage.h"

namespace audit {
namespace upload {

// Google Drive storage
class GdriveStorage : public ReusableStorage {
 public:
  void StoreFile(const std::string& file_name, std::istream& stream,
                 StorageListener& listener) override;

  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    StorageListener& listener) override;

  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         StorageListener& listener) override;
};
}
}
