#pragma once

#include <functional>
#include <string>

#include "cpprest/uri.h"

namespace audit {
namespace dropbox {

class TokenSource;

// This should be used in production in order to make sure that we don't
// authenticate users more than one time
class TokenSourceInstance {
 public:
  static TokenSource& get() { return token_source_; }

 private:
  static TokenSource token_source_;
};

class TokenSource {
 public:
  typedef std::function<std::string(void)> CodeCallbackType;

  TokenSource(CodeCallbackType code_callback);

  std::string GetToken();

 private:
  void OpenAuthorizeUrl() const;

  std::string ExchangeCodeForToken(const std::string& code);

  std::string GetValueFromSecret(const std::string& key);
  std::string GetClientId();
  std::string GetClientSecret();

  const static web::uri AUTHORIZE_URL;
  const static web::uri BASE_URL;
  const static web::uri TOKEN_URL;

  // The full path to the file that contains all the client secrets
  const static std::string SECRETS_FILE;

  const std::string client_id_;
  const std::string client_secret_;

  CodeCallbackType code_callback_;

  std::string token_;

  bool has_token_{false};
};
}
}
