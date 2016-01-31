#include "audit/providers/dropbox/storage.h"

#include "audit/providers/dropbox/file_tag_source.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/proto/cpor.pb.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace dropbox {

const uri DOWNLOAD_PATH{"/2/files/download"};

proto::PrivateFileTag FileTagSource::GetFileTag(const std::string& file_name) {
  json parameters;
  parameters["path"] = Storage::GetFileTagPath(file_name);

  http_request request{"GET"};
  request.set_request_uri(DOWNLOAD_PATH);
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
  auto binary = response.extract_string().get();

  proto::PrivateFileTag tag;
  tag.ParseFromString(binary);
  return tag;
}
}
}
