#pragma once

#include <memory>
#include <iostream>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/client/prf.h"
#include "audit/util.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

// Verifies a file's integrity based on the proof recieved from the server.
// Returns true if the file is intact.
template <typename PRF>
bool VerifyFile(const proto::PrivateFileTag& file_tag,
                const proto::Challenge& challenge, const proto::Proof& proof) {
  PRF prf{file_tag.prf_key()};

  BN_ptr sigma_sum{BN_new(), ::BN_free};
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};

  BN_ptr weighted_index{BN_new(), ::BN_free};
  for (auto& item : challenge.items()) {
    BN_mul(weighted_index.get(), StringToBignum(item.weight()).get(),
           prf.Encode(item.index()).get(), ctx.get());
    BN_add(sigma_sum.get(), sigma_sum.get(), weighted_index.get());
    BN_clear(weighted_index.get());
  }

  for (int i = 0; i < proof.mus_size(); ++i) {
    auto mu = StringToBignum(proof.mus(i));
    BN_mul(mu.get(), mu.get(), StringToBignum(file_tag.alphas(i)).get(),
           ctx.get());
    BN_add(sigma_sum.get(), sigma_sum.get(), mu.get());
  }

  BN_mod(sigma_sum.get(), sigma_sum.get(),
         StringToBignum(file_tag.public_tag().p()).get(), ctx.get());

  return BN_cmp(sigma_sum.get(), StringToBignum(proof.sigma()).get()) == 0;
}
}
}
