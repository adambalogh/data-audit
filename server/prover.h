#pragma once

#include "audit/common.h"

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
};
}
