#include "prover.h"

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/server/fetcher.h"

namespace audit {

proto::Proof Prover::Prove(proto::Challenge chal) {
  BN_ptr sigma_sum{BN_new(), ::BN_free};
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};

  for (auto& challenge : chal.items()) {
    BN_ptr mu{BN_new(), ::BN_free};
    auto tag = fetcher_->FetchBlockTag(challenge.index());
    auto block = fetcher_->FetchBlock(challenge.index());

    auto sigma = StringToBignum(tag.sigma());
    // sigma = sigma * weight
    BN_mul(sigma.get(), sigma.get(), ctx.get());

    // sigma_sum += sigma
    BN_add(sigma_sum.get(), sigma_sum.get(), sigma.get());
  }

  proto::Proof proof;
  for (auto& mu : mus_) {
  }

  return proof;
}
}
