#pragma once

#include "cpprest/uri.h"

#include "audit/providers/dropbox/token_source.h"
#include "cpprest/http_client.h"

#include "audit/providers/dropbox/dropbox_urls.h"

namespace audit {
namespace providers {
namespace dropbox {

// DropboxClient provides a convenient way to access the DropBox API, all
// authentication is handled by it.
//
// This class is not thread safe.
class DropboxClient {
 public:
  DropboxClient(TokenSourceInterface& token_source,
                const web::uri& base_url = BASE_URL)
      : token_source_(token_source), client_(base_url) {}

 protected:
  web::http::http_response SendRequest(web::http::http_request& request);

 private:
  TokenSourceInterface& token_source_;

  web::http::client::http_client client_;
};
}
}
}
