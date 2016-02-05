#pragma once

#include <functional>
#include <string>

#include "cpprest/uri.h"

namespace audit {
namespace providers {
namespace dropbox {

class TokenSourceInterface {
 public:
  virtual std::string GetToken() = 0;

  virtual ~TokenSourceInterface() {}
};

// Returns an OAuth2.0 token that can be used to access the user's Dropbox
// account.
//
// There are 2 flows for getting this token, depending on what the
// NeedToAuthorize() method returns:
//
//   1. If it returns false, the user has already authorized our app with
//      Dropbox before, and the given token has been saved to the token.json
//      file. In this case it automatically loads the token from the file, and
//      it can be accessed with GetToken()
//
//   2. If it returns true, the user has not authorized our app or the token was
//      lost. In this case, we must take the user to the website at the URL
//      GetAuthorizeUrl(). Here the user will receive a code. This code needs to
//      be entered to our app somehow, and ExchangeCodeForToken() can be used to
//      exchange it for a token. This will also save the token to the token.json
//      file.
//
class TokenSource : public TokenSourceInterface {
 public:
  TokenSource();

  // Returns true if the user needs to authorize our app with Dropbox
  bool NeedToAuthorize();

  // Returns the token. It should only be called if NeedToAuthorize() returns
  // false. Otherwise it throws an exception.
  std::string GetToken() override;

  // The user should be shown the webpage at this URL, in order to give access
  // this app access to Dropbox
  std::string GetAuthorizeUrl() const;

  // Exchanges the code received by the user for a token that grants access to
  // Dropbox
  void ExchangeCodeForToken(const std::string& code);

  // The full path to the file that contains all the client secrets
  const static std::string SECRETS_FILE;

  // The fulle path to the file that may contain a token
  const static std::string TOKEN_FILE;

 private:
  bool GetTokenFromFile();
  void SaveTokenToFile();

  std::string GetValueFromSecret(const std::string& key);
  std::string GetClientId();
  std::string GetClientSecret();

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
}
