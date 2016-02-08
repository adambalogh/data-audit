#pragma once

#include <string>

#include "audit/client/verify/file_tag_source.h"
#include "audit/client/verify/proof_source.h"
#include "audit/client/verify/stats.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

// You should only interact with this class for verification.
class Client {
 public:
  Client(std::unique_ptr<FileTagSource> file_tag_source,
         std::unique_ptr<ProofSource> proof_source)
      : file_tag_source_(std::move(file_tag_source)),
        proof_source_(std::move(proof_source)) {}

  bool Verify(const std::string& file_full_path, int percent_blocks,
              Stats& stats);

 private:
  proto::Challenge BuildChallenge(const proto::PublicFileTag& public_tag,
                                  int percent_blocks);

  std::unique_ptr<FileTagSource> file_tag_source_;

  std::unique_ptr<ProofSource> proof_source_;
};
}
}
