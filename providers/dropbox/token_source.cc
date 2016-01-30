#include "audit/providers/dropbox/token_source.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#define BOOST_NETWORK_ENABLE_HTTPS
#include "boost/network.hpp"
#include "boost/network/protocol/http/client.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace boost::network;

namespace audit {
namespace dropbox {

const uri::uri TokenSource::AUTHORIZE_URL{
    "https://www.dropbox.com/1/oauth2/authorize"};

const uri::uri TokenSource::TOKEN_URL{
    "https://api.dropboxapi.com/1/oauth2/token"};

TokenSource::TokenSource()
    : client_id_(GetClientId()), client_secret_(GetClientSecret()) {}

std::string TokenSource::GetClientId() {
  std::ifstream secrets_file{"secrets.json"};
  auto secrets = json::parse(secrets_file);
  return secrets.at("client_id");
}

std::string TokenSource::GetClientSecret() {
  std::ifstream secrets_file{"secrets.json"};
  auto secrets = json::parse(secrets_file);
  return secrets.at("client_secret");
}

std::string TokenSource::ExchangeKeyForToken(const std::string& code) {
  std::stringstream request_body;
  request_body << "code=" << uri::encoded(code)
               << "&grant_type=authorization_code"
               << "&client_id=" << uri::encoded(client_id_)
               << "&client_secret=" << uri::encoded(client_secret_);

  http::client::request request(TOKEN_URL);
  request << header("Content-Type", "application/x-www-form-urlencoded");

  http::client client;
  auto http_response = client.post(request, request_body.str());
  auto response = json::parse(body(http_response));

  auto error = response.find("error");
  if (error != response.end()) {
    std::cout << *error << std::endl;
    std::cout << response["error_description"] << std::endl;
  }

  return response.at("access_token");
}

void TokenSource::OpenAuthorizeURL() const {
  uri::uri authorize_url;
  authorize_url << AUTHORIZE_URL << uri::query("response_type", "code")
                << uri::query("client_id", client_id_);

  std::string command = "open \"" + authorize_url.string() + "\"";
  system(command.c_str());
}

std::string TokenSource::GetToken() { return ""; }
}
}
