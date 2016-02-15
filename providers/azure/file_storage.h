#pragma once

#include <string>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"

#include "audit/client/upload/storage.h"
#include "audit/providers/azure/urls.h"

namespace audit {
namespace providers {
namespace azure {

// FileStorage that stores all the data on an Azure VM's disk.
//
class FileStorage : public upload::FileStorage {
 public:
  void StoreFile(const std::string& file_name, std::istream& stream,
                 audit::upload::StorageListener& listener) override;

 private:
  void UploadFile(const std::string& file_name, std::istream& stream,
                  audit::upload::StorageListener& listener);

  web::http::client::http_client client_{BASE_URL};
};
}
}
}
