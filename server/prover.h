#pragma once

#include "audit/common.h"

#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {
namespace server {

// An ProverInterface implementation should be running directly on the servers
// that contain all our files. On an incoming challenge from the client, it
// should send back the calculated proto::Proof for the requested file to the
// client.
//
class ProverInterface {
 public:
  ProverInterface(Fetcher& fetcher, const proto::Challenge& chal)
      : fetcher_(fetcher), chal_(chal), file_tag_(chal.file_tag()) {}

  virtual proto::Proof Prove() = 0;

  virtual ~ProverInterface() {}

 protected:
  Fetcher& fetcher_;

  const proto::Challenge& chal_;

  const proto::PublicFileTag& file_tag_;
};

class Prover : public ProverInterface {
 public:
  Prover(Fetcher& fetcher, const proto::Challenge& chal);

  // Returns the proof for the requested file and challenge
  proto::Proof Prove() override;

 private:
  // Processes a single challenge and stores the result in sigma_sum and mus
  void ProcessChallenge(const proto::ChallengeItem&);

  // Constructs a proof from mus and sigma_sum
  proto::Proof ConstructProof();

  std::vector<BN_ptr> mus_;

  BN_ptr sigma_sum_{BN_new(), ::BN_free};

  // Used for BN computations
  BN_CTX_ptr ctx_{BN_CTX_new(), ::BN_CTX_free};
};
}
}
