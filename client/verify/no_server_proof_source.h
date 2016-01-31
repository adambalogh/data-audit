#pragma once

#include "audit/client/verify/proof_source.h"
#include "audit/server/fetcher.h"
#include "audit/server/prover.h"

namespace audit {
namespace verify {

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
