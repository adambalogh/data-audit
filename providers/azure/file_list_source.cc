#include "audit/providers/azure/file_list_source.h"

#include "cpprest/http_client.h"

#include "audit/proto/cpor.pb.h"

using web::http::http_request;

namespace audit {
namespace providers {
namespace azure {

using audit::file_browser::File;

std::vector<File> FileListSource::GetFiles() {
  http_request request{"GET"};
  request.set_request_uri(FILELIST_URL);

  auto response = client_.request(request).get();
  auto body_str = response.extract_string().get();

  proto::FileList file_list;
  if (!file_list.ParseFromString(body_str)) {
    // error
  }

  std::vector<File> files;
  for (int i = 0; i < file_list.files_size(); ++i) {
    auto& file = file_list.files(i);
    files.push_back(File{file.name(), file.size()});
  }
  return files;
}
}
}
}
