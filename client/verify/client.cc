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

void GenerateIndexes(int percent_blocks, int num_blocks,
                     std::function<void(int)> callback) {
  // This is the actual number of blocks we will vefify
  int num_blocks_checked =
      std::max(static_cast<double>(num_blocks * percent_blocks) /
                   static_cast<double>(100),
               1.0);
  // Selected blocks should be equally distributed across the whole file
  int selection_range =
      static_cast<double>(num_blocks) / static_cast<double>(num_blocks_checked);

  int start = 0;
  for (int i = 0; i < num_blocks_checked; ++i) {
    int block_index =
        start + (std::rand() % std::min(selection_range, num_blocks - start));
    callback(block_index);
    start += selection_range;
  }
}

// BuildChallenge constructs a proto::Challenge object for the given file.
proto::Challenge Client::BuildChallenge(const proto::PublicFileTag& public_tag,
                                        int percent_blocks) {
  proto::Challenge challenge;
  *challenge.mutable_file_tag() = public_tag;

  BN_ptr max_weight{BN_new(), ::BN_free};
  BN_generate_prime_ex(max_weight.get(), 64, false, NULL, NULL, NULL);
  CryptoNumberGenerator random_gen;

  GenerateIndexes(percent_blocks, public_tag.num_blocks(),
                  [&challenge, &random_gen, &max_weight](int index) {
    auto item = challenge.add_items();
    item->set_weight(BignumToString(*random_gen.GenerateNumber(*max_weight)));
    item->set_index(index);
  });

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

std::vector<bool> Client::BatchVerify(
    const std::vector<const std::string>& files, int percent_blocks,
    StageReportCallback callback, Stats& stats) {
  std::vector<bool> results(files.size(), false);
  std::vector<proto::Challenge> challenges(files.size());
  std::vector<proto::PrivateFileTag> file_tags(files.size());

  BN_ptr max_weight{BN_new(), ::BN_free};
  BN_generate_prime_ex(max_weight.get(), 64, false, NULL, NULL, NULL);
  CryptoNumberGenerator random_gen;

  proto::BatchChallenge batch_challenge;
  for (int i = 0; i < files.size(); ++i) {
    auto file_name = files.at(i);
    file_tags[i] = file_tag_source_.GetFileTag(file_name);
    auto& file_tag = file_tags[i];
    auto batch_challenge_item = batch_challenge.add_challenges();
    *batch_challenge_item->mutable_file_tag() = file_tag.public_tag();
    *challenges[i].mutable_file_tag() = file_tag.public_tag();

    GenerateIndexes(percent_blocks, file_tag.public_tag().num_blocks(),
                    [&](int index) {
      batch_challenge_item->add_indexes(index);
      if (batch_challenge_item->indexes_size() >
          batch_challenge.weights_size()) {
        *batch_challenge.add_weights() =
            BignumToString(*random_gen.GenerateNumber(*max_weight));
      }
      assert(batch_challenge_item->indexes_size() <=
             batch_challenge.weights_size());

      auto challenge_item = challenges[i].add_items();
      challenge_item->set_index(index);
      challenge_item->set_weight(
          batch_challenge.weights(challenges[i].items_size() - 1));
    });
  }

  auto proofs = proof_source_->BatchGetProof(batch_challenge);
  for (int i = 0; i < proofs.proofs_size(); ++i) {
    results[i] = VerifyFile<upload::Client::PrfType>(
        file_tags[i], challenges[i], proofs.proofs(i));
  }

  assert(results.size() == files.size());
  return results;
}
}
}
