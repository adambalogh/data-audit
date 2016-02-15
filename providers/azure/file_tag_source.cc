#include "audit/providers/azure/file_tag_source.h"

#include <fstream>
#include <sstream>

#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/proto/cpor.pb.h"
#include "audit/providers/azure/urls.h"

using json = nlohmann::json;

using web::http::http_request;

namespace audit {
namespace providers {
namespace azure {

proto::PrivateFileTag FileTagSource::GetFileTag(const std::string& file_name) {
  json params;
  params["file_name"] = file_name;

  http_request request{"GET"};
  request.set_request_uri(FILETAG_URL);
  request.set_body(params.dump());

  auto response = client_.request(request).get();
  auto tag_str = response.extract_string().get();

  proto::PrivateFileTag tag;
  if (!tag.ParseFromString(tag_str)) {
    // error
  }
  return tag;
}
}
}
}
