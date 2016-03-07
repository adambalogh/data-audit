#pragma once

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

// ProofSource should return a proof constructed with audit::server::Prover.
//
// Usually, it will reach out to the Cloud Service Provider, which will
// calculate and return the Proof using its local copy of the file.
class ProofSource {
 public:
  virtual proto::Proof GetProof(const proto::Challenge& challenge) = 0;

  // BatchGetProof is a more efficient way of verifying multiple files at the
  // same time
  virtual proto::BatchProof BatchGetProof(
      const proto::BatchChallenge& challenge) = 0;

  virtual ~ProofSource() {}
};
}
}
