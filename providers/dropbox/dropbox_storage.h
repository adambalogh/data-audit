#pragma once

#include "audit/client/upload/storage.h"
#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/dropbox_client.h"

#include "cpprest/uri.h"

namespace audit {
namespace upload {

// Dropbox storage
class DropboxStorage : public dropbox::DropboxClient, public ReusableStorage {
 public:
  DropboxStorage(dropbox::TokenSource& token_source)
      : DropboxClient(token_source) {}

  void StoreFile(const std::string& file_name, std::istream& stream,
                 StorageListener& listener) override;

  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    StorageListener& listener) override;

  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         StorageListener& listener) override;

 private:
  void SaveFileToDropbox(const std::string& path, std::istream& stream,
                         StorageListener& listener);

  const static web::uri UPLOAD_URL;
};
}
}
