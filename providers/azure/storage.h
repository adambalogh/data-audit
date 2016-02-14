#pragma once

#include <string>

#include "cpprest/uri.h"

#include "audit/client/upload/storage.h"

namespace audit {
namespace providers {
namespace azure {

// Storage implementation that stores all the data on an Azure VM.
//
class Storage : public upload::ReusableStorage {
 public:
  Storage(const web::uri& host_uri) : host_url_(host_uri) {}

  void StoreFile(const std::string& file_name, std::istream& stream,
                 ::audit::upload::StorageListener& listener) override;

  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    ::audit::upload::StorageListener& listener) override;

  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         audit::upload::StorageListener& listener) override;

  static std::string GetFilePath(const std::string& file_name) {
    return file_name;
  }

  static std::string GetFileTagPath(const std::string& file_name) {
    return "file_tag_" + file_name;
  }

  static std::string GetBlockTagFilePath(const std::string& file_name) {
    return "tags_file_" + file_name;
  }

 private:
  void UploadFile(const std::string& file_name, std::istream& stream,
                  ::audit::upload::StorageListener& listener);

  const web::uri host_url_;
};
}
}
}
