#include "audit/providers/dropbox/file_tag_source.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/proto/cpor.pb.h"
#include "audit/providers/dropbox/dropbox_urls.h"
#include "audit/providers/dropbox/file_storage.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace providers {
namespace dropbox {

std::vector<uint8_t> FileTagSource::GetFileTag(const std::string& file_name) {
  json parameters;
  parameters["path"] = "/" + upload::Storage::GetFileTagPath(file_name);

  http_request request{"GET"};
  request.set_request_uri(DOWNLOAD_PATH);
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
  return response.extract_vector().get();
}
}
}
}
