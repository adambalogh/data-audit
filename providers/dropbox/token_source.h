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
  typedef std::function<std::string(void)> CodeCallbackType;

  TokenSource(CodeCallbackType code_callback);

  std::string GetToken() override;

 private:
  void OpenAuthorizeUrl() const;

  std::string ExchangeCodeForToken(const std::string& code);

  std::string GetValueFromSecret(const std::string& key);
  std::string GetClientId();
  std::string GetClientSecret();

  // The full path to the file that contains all the client secrets
  const static std::string SECRETS_FILE;

  const std::string client_id_;
  const std::string client_secret_;

  CodeCallbackType code_callback_;

  std::string token_;

  bool has_token_{false};
};

// This should be used in order to make sure that we don't
// authenticate users more than one time
class TokenSourceInstance {
 public:
  // Initialize must be called before any calls to Get()
  static void Initialize(TokenSource::CodeCallbackType code_callback) {
    token_source_ = new TokenSource(code_callback);
  }

  ~TokenSourceInstance() { delete token_source_; }

  static TokenSource& Get() { return *token_source_; }

 private:
  static TokenSource* token_source_;
};
}
}
