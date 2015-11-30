#include "prover.h"

#include <assert.h>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {

proto::Proof Prover::MakeProof(BIGNUM* sigma_sum,
                               const std::vector<BN_ptr>& mus) {
  proto::Proof proof;
  BignumToString(*sigma_sum, proof.mutable_sigma());
  for (auto& mu : mus) {
    proof.add_mus(BignumToString(*mu));
  }
  return proof;
}

proto::Proof Prover::Prove(Fetcher& fetcher, const proto::Challenge& chal) {
  auto& file_tag = chal.file_tag();

  std::vector<BN_ptr> mus;
  std::generate_n(std::back_inserter(mus), file_tag.num_sectors(), []() {
    return std::move(BN_ptr{BN_new(), ::BN_free});
  });

  BN_ptr sigma_sum{BN_new(), ::BN_free};

  for (auto& challenge : chal.items()) {
    const auto tag = fetcher.FetchBlockTag(challenge.index());
    auto& block_stream = fetcher.FetchBlock(challenge.index());

    auto weight = StringToBignum(challenge.weight());
    auto sigma = StringToBignum(tag.sigma());

    BN_mul(sigma.get(), sigma.get(), weight.get(), ctx_.get());
    BN_add(sigma_sum.get(), sigma_sum.get(), sigma.get());

    std::vector<unsigned char> block;
    block.resize(file_tag.sector_size());
    BN_ptr content{BN_new(), ::BN_free};

    for (int i = 0; i < file_tag.num_sectors(); ++i) {
      block_stream.read((char*)block.data(), file_tag.sector_size());
      assert(block_stream.gcount() == file_tag.sector_size() ||
             i + 1 == file_tag.num_sectors());
      BN_bin2bn(&block[0], block_stream.gcount(), content.get());
      BN_mul(content.get(), content.get(), weight.get(), ctx_.get());
      BN_add(mus.at(i).get(), mus.at(i).get(), content.get());
    }
  }
  auto p = StringToBignum(file_tag.p());
  BN_mod(sigma_sum.get(), sigma_sum.get(), p.get(), ctx_.get());
  for (auto& mu : mus) {
    BN_mod(mu.get(), mu.get(), p.get(), ctx_.get());
  }

  return MakeProof(sigma_sum.get(), mus);
}
}
