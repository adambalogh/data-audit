#include "audit/providers/dropbox/dropbox_client.h"

#include <string>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using web::uri;
using web::http::http_response;
using web::http::http_request;
using web::http::client::http_client;

namespace audit {
namespace dropbox {

http_response DropboxClient::SendRequest(http_request& request) {
  request.headers().add("Authorization", "Bearer " + token_source_.GetToken());
  auto response = client_.request(request).get();

  if (response.status_code() < 200 || response.status_code() >= 300) {
    auto response_body = json::parse(response.extract_string().get());

    throw std::runtime_error(
        "Sent unsuccessful request to Dropbox. HTTP status code: " +
        std::to_string(response.status_code()) + ". Error message: " +
        response_body["error_summary"].get<std::string>());
  }

  return response;
}
}
}
