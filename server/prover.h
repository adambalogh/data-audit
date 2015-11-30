#pragma once

#include "audit/common.h"

#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {

class ProverInterface {
 public:
  virtual proto::Proof Prove(Fetcher& fetcher,
                             const proto::Challenge& chal) = 0;
};

class Prover : public ProverInterface {
 public:
  proto::Proof Prove(Fetcher& fetcher, const proto::Challenge& chal) override;

 private:
  proto::Proof MakeProof(BIGNUM*, const std::vector<BN_ptr>&);

  BN_CTX_ptr ctx_{BN_CTX_new(), ::BN_CTX_free};
};
}
