#include "audit/client/verification.h"

#include "openssl/bn.h"

#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

bool Verification::Verify(const proto::PrivateFileTag& file_tag,
                          const proto::Challenge& challenge,
                          const proto::Proof& proof, PRF& prf) {
  BN_ptr sigma_sum{BN_new(), ::BN_free};
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};

  BN_ptr weighted_index{BN_new(), ::BN_free};
  for (auto& item : challenge.items()) {
    BN_mul(weighted_index.get(), StringToBignum(item.weight()).get(),
           prf.Encode(item.index()).get(), ctx.get());
    BN_add(sigma_sum.get(), sigma_sum.get(), weighted_index.get());
    BN_clear(weighted_index.get());
  }
}
}
