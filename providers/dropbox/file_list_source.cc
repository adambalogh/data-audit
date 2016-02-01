#include "audit/providers/dropbox/file_list_source.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/providers/dropbox/dropbox_urls.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace dropbox {

std::vector<const std::string> FileListSource::GetFiles() {
  json parameters;
  parameters["path"] = "";
  parameters["recursive"] = false;
  parameters["include_media_info"] = false;
  parameters["include_deleted"] = false;

  http_request request{"POST"};
  request.headers().add("Content-Type", "application/json");
  request.set_request_uri(LIST_FOLDER_PATH);
  request.set_body(parameters.dump());

  auto response = SendRequest(request);
  auto response_body = json::parse(response.extract_string().get());

  // TODO follow cursor if has_more is true
  auto entries = response_body.at("entries");

  std::vector<const std::string> files;
  for (auto& entry : entries) {
    if (entry.at(".tag") != "file") {
      continue;
    }
    files.push_back(entry.at("name"));
  }

  return files;
}
}
}
