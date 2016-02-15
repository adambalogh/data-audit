#pragma once

#include "cpprest/http_client.h"

#include "audit/files/file_list_source.h"
#include "audit/providers/azure/urls.h"

namespace audit {
namespace providers {
namespace azure {

class FileListSource : public audit::file_browser::FileListSource {
 public:
  std::vector<audit::file_browser::File> GetFiles();

 private:
  web::http::client::http_client client_{BASE_URL};
};
}
}
}
