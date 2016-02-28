#include "audit/client/verify/client.h"

#include <cstdlib>
#include <chrono>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/client/verify/verify_file.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

// BuildChallenge constructs a proto::Challenge object for the given file.
proto::Challenge Client::BuildChallenge(const proto::PublicFileTag& public_tag,
                                        int percent_blocks) {
  proto::Challenge challenge;
  *challenge.mutable_file_tag() = public_tag;

  // This is the actual number of blocks we will vefify
  int num_blocks_checked =
      std::max(static_cast<double>((public_tag.num_blocks() * percent_blocks)) /
                   static_cast<double>(100),
               1.0);

  BN_ptr max_weight{BN_new(), ::BN_free};
  BN_generate_prime_ex(max_weight.get(), 64, false, NULL, NULL, NULL);

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

bool Client::DoVerify(const std::string& file_name, int percent_blocks,
                      StageReportCallback callback, Stats& stats) {
  if (percent_blocks < 0 || percent_blocks > 100) {
    throw std::logic_error(
        "The percentage of blocks checked must be between 0 and 100.");
  }

  callback("Preparing verification request...");
  auto file_tag = file_tag_source_.GetFileTag(file_name);
  auto challenge = BuildChallenge(file_tag.public_tag(), percent_blocks);
  callback("Waiting for server's response...");
  auto proof = proof_source_->GetProof(challenge);

  stats.file_tag_size = static_cast<size_t>(file_tag.ByteSize());
  stats.challenge_size = static_cast<size_t>(challenge.ByteSize());
  stats.proof_size = static_cast<size_t>(proof.ByteSize());

  callback("Verifying server's response...");
  return VerifyFile<upload::Client::PrfType>(file_tag, challenge, proof);
}

bool Client::Verify(const std::string& file_name, int percent_blocks,
                    StageReportCallback callback, Stats& stats) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = DoVerify(file_name, percent_blocks, callback, stats);
  auto duration = std::chrono::high_resolution_clock::now() - start;

  stats.milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  return result;
}
}
}
