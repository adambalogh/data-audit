#pragma once

#include "audit/common.h"

#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {

class ProverInterface {
 public:
  ProverInterface(Fetcher& fetcher, const proto::Challenge& chal)
      : fetcher_(fetcher), chal_(chal), file_tag_(chal.file_tag()) {}

  virtual proto::Proof Prove() = 0;

 protected:
  Fetcher& fetcher_;
  const proto::Challenge& chal_;
  const proto::PublicFileTag& file_tag_;
};

class Prover : public ProverInterface {
 public:
  Prover(Fetcher& fetcher, const proto::Challenge& chal);

  proto::Proof Prove() override;

 private:
  void ProcessChallenge(const proto::ChallengeItem&);
  proto::Proof ConstructProof();

  std::vector<BN_ptr> mus_;
  BN_ptr sigma_sum_{BN_new(), ::BN_free};
  BN_CTX_ptr ctx_{BN_CTX_new(), ::BN_CTX_free};
};
}
