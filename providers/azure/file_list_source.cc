#include "audit/providers/azure/file_list_source.h"

#include "cpprest/http_client.h"

using web::http::http_request;

namespace audit {
namespace providers {
namespace azure {

using audit::file_browser::File;

std::vector<File> FileListSource::GetFiles() {
  http_request request{"GET"};
  request.set_request_uri(FILELIST_URL);

  std::vector<File> files;
  return files;
}
}
}
}
