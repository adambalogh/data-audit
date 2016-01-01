#pragma once

#include "audit/client/verify/proof_source.h"

namespace audit {
namespace verify {

class LocalProofSource : public ProofSource {
 public:
  virtual proto::Proof GetProof(const proto::Challenge& challenge);
};
}
}
