#pragma once

#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {

class ProverInterface {
 public:
  virtual proto::Proof Prove(proto::Challenge chal) = 0;
};

class Prover : public ProverInterface {
 public:
  proto::Proof Prove(proto::Challenge chal) override;

 private:
  std::vector<BN_ptr> mus_;
  std::unique_ptr<Fetcher> fetcher_;
}
}
