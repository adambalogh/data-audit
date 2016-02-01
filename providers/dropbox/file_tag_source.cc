#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/file_tag_source.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/proto/cpor.pb.h"
#include "audit/providers/dropbox/dropbox_urls.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace dropbox {

proto::PrivateFileTag FileTagSource::GetFileTag(const std::string& file_name) {
  json parameters;
  parameters["path"] = Storage::GetFileTagPath(file_name);

  http_request request{"GET"};
  request.set_request_uri(DOWNLOAD_PATH);
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
  auto binary = response.extract_vector().get();

  proto::PrivateFileTag tag;
  tag.ParseFromArray(binary.data(), binary.size());

  return tag;
}
}
}
