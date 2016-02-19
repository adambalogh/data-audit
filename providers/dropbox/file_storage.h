#pragma once

#include "audit/client/upload/storage.h"
#include "audit/providers/dropbox/dropbox_client.h"

#include "cpprest/uri.h"

namespace audit {
namespace providers {
namespace dropbox {

// Dropbox storage
class FileStorage : public DropboxClient, public upload::FileStorage {
 public:
  FileStorage(TokenSourceInterface& token_source)
      : DropboxClient(token_source) {}

  void StoreFile(const std::string& path, std::istream& stream,
                 upload::StorageListener& listener) {
    SaveFileToDropbox(path, stream, listener);
  }

 private:
  void SaveFileToDropbox(const std::string& path, std::istream& stream,
                         upload::StorageListener& listener);

  const static web::uri UPLOAD_URL;
};
}
}
}
