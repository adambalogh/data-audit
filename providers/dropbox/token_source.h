#pragma once

#include <functional>
#include <string>

#include "cpprest/uri.h"

namespace audit {
namespace dropbox {

class TokenSourceInterface {
 public:
  virtual std::string GetToken() = 0;

  virtual ~TokenSourceInterface() {}
};

class TokenSource : public TokenSourceInterface {
 public:
  TokenSource();

  std::string GetAuthorizeUrl() const;

  void ExchangeCodeForToken(const std::string& code);

  std::string GetToken() override;

 private:
  std::string GetValueFromSecret(const std::string& key);
  std::string GetClientId();
  std::string GetClientSecret();

  // The full path to the file that contains all the client secrets
  const static std::string SECRETS_FILE;

  const std::string client_id_;
  const std::string client_secret_;

  std::string token_;

  bool has_token_{false};
};

// This should be used in order to make sure that we don't
// authenticate users more than one time
class TokenSourceInstance {
 public:
  static TokenSource& Get() { return token_source_; }

 private:
  static TokenSource token_source_;
};
}
}
