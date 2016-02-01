#include "audit/providers/dropbox/token_source.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "nlohmann/json.hpp"

#include "audit/providers/dropbox/dropbox_urls.h"

using json = nlohmann::json;

using web::uri;
using web::uri_builder;
using web::http::client::http_client;
using web::http::http_request;

namespace audit {
namespace dropbox {

const std::string TokenSource::SECRETS_FILE{
    "/users/adambalogh/Developer/audit/providers/dropbox/secrets.json"};

std::string TokenSource::GetValueFromSecret(const std::string& key) {
  std::ifstream secrets_file{SECRETS_FILE};
  if (!secrets_file) {
    throw std::runtime_error(
        "Could not open file containing Dropbox client secrets (" +
        SECRETS_FILE + ")");
  }
  auto secrets = json::parse(secrets_file);
  return secrets.at(key);
}

std::string TokenSource::GetClientId() {
  return GetValueFromSecret("client_id");
}

std::string TokenSource::GetClientSecret() {
  return GetValueFromSecret("client_secret");
}

TokenSource::TokenSource()
    : client_id_(GetClientId()), client_secret_(GetClientSecret()) {}

std::string TokenSource::ExchangeCodeForToken(const std::string& code) {
  std::stringstream request_body;
  request_body << "code=" << uri::encode_data_string(code)
               << "&grant_type=authorization_code"
               << "&client_id=" << uri::encode_data_string(client_id_)
               << "&client_secret=" << uri::encode_data_string(client_secret_);

  http_request request("POST");
  request.set_request_uri(TOKEN_PATH);
  request.headers().add("Content-Type", "application/x-www-form-urlencoded");
  request.set_body(request_body.str());

  http_client client{AUTH_URL};
  auto http_response = client.request(request).get();
  auto response = json::parse(http_response.extract_string().get());

  // If request was invalid, "error" field will be set
  auto error = response.find("error");
  if (error != response.end()) {
    throw std::runtime_error("Unable to authorize app with Dropbox: " +
                             (*error).get<std::string>() + ". Description: " +
                             response["error_description"].get<std::string>());
  }

  return response.at("access_token");
}

void TokenSource::OpenAuthorizeUrl() const {
  uri_builder builder{AUTHORIZE_URL};
  builder.append_query("response_type", "code")
      .append_query("client_id", client_id_);
  auto authorize_url = builder.to_uri();

  std::string command = "open \"" + authorize_url.to_string() + "\"";
  system(command.c_str());
}

std::string TokenSource::GetToken() {
  if (!has_token_) {
    OpenAuthorizeUrl();
    auto code = code_callback_();
    token_ = ExchangeCodeForToken(code);
    has_token_ = true;
  }
  return token_;
}

TokenSource TokenSourceInstance::token_source_{};
}
}
