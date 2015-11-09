#include "prover.h"

#include <assert.h>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/server/fetcher.h"

namespace audit {

proto::Proof Prover::Prove(const proto::PublicFileTag& file_tag,
                           Fetcher& fetcher, const proto::Challenge& chal) {
  std::vector<BN_ptr> mus;
  std::generate_n(std::back_inserter(mus), file_tag.num_sectors(), []() {
    return std::move(BN_ptr{BN_new(), ::BN_free});
  });

  BN_ptr sigma_sum{BN_new(), ::BN_free};
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};

  for (auto& challenge : chal.items()) {
    const auto tag = fetcher.FetchBlockTag(challenge.index());
    auto& block_stream = fetcher.FetchBlock(challenge.index());

    auto weight = StringToBignum(challenge.weight());
    auto sigma = StringToBignum(tag.sigma());

    // sigma = sigma * weight
    BN_mul(sigma.get(), sigma.get(), weight.get(), ctx.get());
    // sigma_sum += sigma
    BN_add(sigma_sum.get(), sigma_sum.get(), sigma.get());

    std::vector<unsigned char> block;
    block.resize(file_tag.sector_size());
    BN_ptr content{BN_new(), ::BN_free};

    for (int i = 0; i < file_tag.num_sectors(); ++i) {
      block_stream.read((char*)block.data(), file_tag.sector_size());
      assert(block_stream.gcount() == file_tag.sector_size() ||
             i + 1 == file_tag.num_sectors());
      BN_bin2bn(&block[0], block_stream.gcount(), content.get());
      // content = content * weight
      BN_mul(content.get(), content.get(), weight.get(), ctx.get());
      // mus[i] += content
      BN_add(mus.at(i).get(), mus.at(i).get(), content.get());
    }
  }
  auto p = StringToBignum(file_tag.public.p());

  proto::Proof proof;
  // sigma_sum = sigma_sum % p
  BN_mod(sigma_sum.get(), sigma_sum.get(), p.get(), ctx.get());
  BignumToString(*sigma_sum, proof.mutable_sigma());
  for (auto& mu : mus) {
    // mus[i] = mus[i] % p
    BN_mod(mu.get(), mu.get(), p.get(), ctx.get());
    proof.add_mus(BignumToString(*mu));
  }

  return proof;
}
}
