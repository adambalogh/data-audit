#pragma once

#include "audit/providers/dropbox/token_source.h"

namespace audit {
namespace providers {
namespace dropbox {

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
