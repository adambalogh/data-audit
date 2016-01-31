#include "audit/providers/dropbox/dropbox_client.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"

using web::uri;
using web::http::http_response;
using web::http::http_request;
using web::http::client::http_client;

namespace audit {
namespace dropbox {

const uri DropboxClient::BASE_URL{"https://content.dropboxapi.com"};

http_response DropboxClient::SendRequest(http_request& request) {
  request.headers().add("Authorization", "Bearer " + token_source_.GetToken());
  auto future_response = client_.request(request);
  return future_response.get();
}
}
}
