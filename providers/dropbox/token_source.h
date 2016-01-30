#pragma once

#include <functional>
#include <string>

#define BOOST_NETWORK_ENABLE_HTTPS

#include "boost/network.hpp"
#include "boost/network/protocol/http/client.hpp"

namespace audit {
namespace dropbox {

class TokenSource {
 public:
  typedef std::function<std::string(void)> CodeCallbackType;

  TokenSource(CodeCallbackType code_callback);

  std::string GetToken();

 private:
  void OpenAuthorizeUrl() const;

  std::string ExchangeCodeForToken(const std::string& code);

  std::string GetClientId();
  std::string GetClientSecret();

  const static boost::network::uri::uri AUTHORIZE_URL;
  const static boost::network::uri::uri TOKEN_URL;

  // The full path to the file that contains all the client secrets
  const static std::string SECRETS_FILE;

  const std::string client_id_;
  const std::string client_secret_;

  CodeCallbackType code_callback_;
};
}
}
