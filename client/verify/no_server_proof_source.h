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
  NoServerProofSource(std::unique_ptr<server::FetcherFactory> fetcher_factory)
      : fetcher_factory_(std::move(fetcher_factory)) {}

  virtual proto::Proof GetProof(const proto::Challenge& challenge) {
    auto fetcher = fetcher_factory_->Create(challenge.file_tag());
    server::Prover prover{*fetcher, challenge};
    return prover.Prove();
  }

 private:
  std::unique_ptr<server::FetcherFactory> fetcher_factory_;
};
}
}
