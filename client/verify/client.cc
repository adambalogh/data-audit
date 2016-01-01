#include "audit/client/verify/client.h"

#include <cstdlib>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/verify/verify_file.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

proto::Challenge Client::BuildChallenge(const proto::PublicFileTag& public_tag,
                                        int percent_blocks) {
  proto::Challenge challenge;
  *challenge.mutable_file_tag() = public_tag;
  int num_blocks_checked =
      static_cast<double>((public_tag.num_blocks() * percent_blocks)) /
      static_cast<double>(100);

  BN_ptr max_weight{BN_new(), ::BN_free};
  BN_generate_prime_ex(max_weight.get(), 512, false, NULL, NULL, NULL);

  CryptoNumberGenerator random_gen;

  // Selected blocks should be equally distributed across the whole file
  int selection_range = static_cast<double>(public_tag.num_blocks()) /
                        static_cast<double>(num_blocks_checked);
  int start = 0;
  for (int i = 0; i < num_blocks_checked; ++i) {
    auto item = challenge.add_items();
    item->set_weight(BignumToString(*random_gen.GenerateNumber(*max_weight)));
    int block_index =
        start + (std::rand() %
                 std::min(selection_range, public_tag.num_blocks() - start));
    item->set_index(block_index);
    start += selection_range;
  }
  return challenge;
}

bool Client::Verify(const std::string& file_full_path, int percent_blocks) {
  if (percent_blocks < 0 || percent_blocks > 100) {
    throw std::logic_error(
        "The percentage of blocks checked must be between 0 and 100.");
  }

  auto file_tag = file_tag_source_->GetFileTag(file_full_path);
  auto challenge = BuildChallenge(file_tag.public_tag(), percent_blocks);
  auto proof = proof_source_->GetProof(challenge);
  return VerifyFile<audit::HMACPRF>(file_tag, challenge, proof);
}
}
}
