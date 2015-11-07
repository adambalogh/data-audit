#include "prover.h"

namespace audit {

proto::Proof Prover::Prove(proto::Challenge chal) {
  for (auto challenge : chal.items()) {
    auto tag = fetcher_->FetchTag(challenge.index());
    auto block = fetcher_->FetchBlock(challenge.index());
  }
}
}
