#pragma once

#include "audit/client/verify/proof_source.h"
#include "audit/server/fetcher.h"
#include "audit/server/prover.h"

namespace audit {
namespace verify {

// In theory, the proof should come from the server that contains the
// requested file, however this is not possible if we want to use this system
// with actual cloud storage providers like Dropbox, because of the limitations
// of the API. As a result, we need to download the file and tags, and generate
// the proof locally.
//
class NoServerProofSource : public ProofSource {
 public:
  NoServerProofSource(std::unique_ptr<server::FetcherFactory> fetcher_factory)
      : fetcher_factory_(std::move(fetcher_factory)) {}

  virtual proto::Proof GetProof(const proto::Challenge& challenge) {
    auto fetcher = fetcher_factory_->Create(challenge.file_tag());
    fetcher->Setup();
    server::Prover prover{*fetcher, challenge};
    return prover.Prove();
  }

 private:
  std::unique_ptr<server::FetcherFactory> fetcher_factory_;
};
}
}
