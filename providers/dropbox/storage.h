#pragma once

#include "audit/client/upload/storage.h"
#include "audit/providers/dropbox/dropbox_client.h"

#include "cpprest/uri.h"

namespace audit {
namespace dropbox {

// Dropbox storage
class Storage : public DropboxClient, public upload::ReusableStorage {
 public:
  Storage(TokenSourceInterface& token_source) : DropboxClient(token_source) {}

  void StoreFile(const std::string& file_name, std::istream& stream,
                 upload::StorageListener& listener) override;

  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    upload::StorageListener& listener) override;

  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         upload::StorageListener& listener) override;

  // Returns the full path to the file where the given file is stored
  static std::string GetFilePath(const std::string& file_name) {
    return "/" + file_name;
  }

  // Returns the full path to the file where the PrivateFileTag of the given
  // file is stored
  static std::string GetFileTagPath(const std::string& file_name) {
    return "/FileTags/" + file_name;
  }

  // Returns the full path to the fule where all the BlockTags are stored for
  // the given file
  static std::string GetBlockTagFilePath(const std::string& file_name) {
    return "/Tags/" + file_name;
  }

 private:
  void SaveFileToDropbox(const std::string& path, std::istream& stream,
                         upload::StorageListener& listener);

  const static web::uri UPLOAD_URL;
};
}
}
