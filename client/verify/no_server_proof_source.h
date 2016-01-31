#pragma once

#include "audit/client/verify/proof_source.h"
#include "audit/server/fetcher.h"
#include "audit/server/prover.h"

namespace audit {
namespace verify {

// In theory, the proof should come from the server that contains the
// requested file, however this is impossible, since we cannot run our own
// code on cloud storage services. As a result, we need to run the proof
// generating code locally. This class does this.
//
class NoServerProofSource : public ProofSource {
 public:
  NoServerProofSource(std::unique_ptr<Fetcher> fetcher)
      : fetcher_(std::move(fetcher)) {}

  virtual proto::Proof GetProof(const proto::Challenge& challenge) {
    Prover prover{*fetcher_, challenge};
    return prover.Prove();
  }

 private:
  std::unique_ptr<Fetcher> fetcher_;
};
}
}
