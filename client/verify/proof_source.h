#pragma once

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

class ProofSource {
 public:
  virtual proto::Proof GetProof(const proto::Challenge& challenge) = 0;

  virtual ~ProofSource() {}
};
}
}
