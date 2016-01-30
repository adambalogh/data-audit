#pragma once

#include <string>

#include "boost/network.hpp"
#include "boost/network/protocol/http/client.hpp"

namespace audit {
namespace dropbox {

class TokenSource {
 public:
  TokenSource();

  std::string GetToken();

 private:
  void OpenAuthorizeURL() const;

  std::string ExchangeKeyForToken(const std::string& code);

  std::string GetClientId();
  std::string GetClientSecret();

  const static boost::network::uri::uri AUTHORIZE_URL;
  const static boost::network::uri::uri TOKEN_URL;

  const std::string client_id_;
  const std::string client_secret_;
};
}
}
