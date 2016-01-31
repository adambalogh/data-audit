#include "prover.h"

#include <assert.h>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/proto/cpor.pb.h"
#include "audit/server/fetcher.h"

namespace audit {
namespace server {

Prover::Prover(Fetcher& fetcher, const proto::Challenge& chal)
    : ProverInterface(fetcher, chal) {
  std::generate_n(std::back_inserter(mus_), file_tag_.num_sectors(), []() {
    return std::move(BN_ptr{BN_new(), ::BN_free});
  });
}

proto::Proof Prover::ConstructProof() {
  proto::Proof proof;
  BignumToString(*sigma_sum_, proof.mutable_sigma());
  for (const auto& mu : mus_) {
    proof.add_mus(BignumToString(*mu));
  }
  return proof;
}

void Prover::ProcessChallenge(const proto::ChallengeItem& challenge) {
  const auto tag = fetcher_.FetchBlockTag(chal_.file_tag(), challenge.index());
  auto block_stream = fetcher_.FetchBlock(chal_.file_tag(), challenge.index());

  auto weight = StringToBignum(challenge.weight());
  auto sigma = StringToBignum(tag.sigma());

  BN_mul(sigma.get(), sigma.get(), weight.get(), ctx_.get());
  BN_add(sigma_sum_.get(), sigma_sum_.get(), sigma.get());

  std::vector<unsigned char> block;
  block.resize(file_tag_.sector_size());
  BN_ptr content{BN_new(), ::BN_free};

  for (int i = 0; i < file_tag_.num_sectors(); ++i) {
    block_stream->read((char*)block.data(), file_tag_.sector_size());
    if (!block_stream->gcount()) {
      break;
    }

    BN_bin2bn(&block[0], block_stream->gcount(), content.get());
    BN_mul(content.get(), content.get(), weight.get(), ctx_.get());
    BN_add(mus_.at(i).get(), mus_.at(i).get(), content.get());
    if (block_stream->gcount() < file_tag_.sector_size()) {
      break;
    }
  }
}

proto::Proof Prover::Prove() {
  for (auto& challenge : chal_.items()) {
    ProcessChallenge(challenge);
  }

  auto p = StringToBignum(file_tag_.p());
  BN_mod(sigma_sum_.get(), sigma_sum_.get(), p.get(), ctx_.get());
  for (auto& mu : mus_) {
    BN_mod(mu.get(), mu.get(), p.get(), ctx_.get());
  }

  return ConstructProof();
}
}
}
