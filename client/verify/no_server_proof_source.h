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

  virtual proto::BatchProof BatchGetProof(
      const proto::BatchChallenge& batch_challenge) {
    proto::BatchProof proofs;

    for (const auto& batch_item : batch_challenge.challenges()) {
      proto::Challenge challenge;
      // TODO avoid copying file_tag and weight
      *challenge.mutable_file_tag() = batch_item.file_tag();
      for (int index_i = 0; index_i < batch_item.indexes_size(); ++index_i) {
        auto challenge_item = challenge.add_items();
        challenge_item->set_index(batch_item.indexes(index_i));
        assert(batch_challenge.weights_size() >= index_i);
        challenge_item->set_weight(batch_challenge.weights(index_i));
      }
      *proofs.add_proofs() = std::move(GetProof(challenge));
    }

    return std::move(proofs);
  }

 private:
  std::unique_ptr<server::FetcherFactory> fetcher_factory_;
};
}
}
