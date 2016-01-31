#pragma once

#include "cpprest/uri.h"

#include "audit/providers/dropbox/token_source.h"
#include "cpprest/http_client.h"

#include "audit/providers/dropbox/dropbox_urls.h"

namespace audit {
namespace dropbox {

// DropboxClient provides a convenient way to access the DropBox API, all
// authentication is handled by it.
//
// This class is not thread safe.
class DropboxClient {
 public:
  DropboxClient(TokenSourceInterface& token_source)
      : token_source_(token_source) {}

 protected:
  web::http::http_response SendRequest(web::http::http_request& request);

 private:
  web::http::client::http_client client_{BASE_URL};

  TokenSourceInterface& token_source_;
};
}
}
