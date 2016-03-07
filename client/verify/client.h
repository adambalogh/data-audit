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
  typedef std::function<void(std::string)> StageReportCallback;

  Client(std::unique_ptr<BinaryFileTagSource> binary_file_tag_source,
         std::unique_ptr<ProofSource> proof_source)
      : file_tag_source_(std::move(binary_file_tag_source)),
        proof_source_(std::move(proof_source)) {}

  // Verify returns true if the given file's integrity could be verified.
  //
  // @param file_name: the name of the file we want to verify
  // @param percent_blocks: this many percentage of the blocks in the file will
  //   be verified. The blocks are randomly selected, but they will have an
  //   equal distribution.
  // @param stats: stats related to the verification will be placed into this
  //   object
  //
  bool Verify(const std::string& file_name, int percent_blocks,
              StageReportCallback callback, Stats& stats);

  std::vector<bool> BatchVerify(const std::vector<const std::string>& files,
                                int percent_blocks,
                                StageReportCallback callback, Stats& stats);

 private:
  bool DoVerify(const std::string& file_name, int percent_blocks,
                StageReportCallback callback, Stats& stats);

  proto::Challenge BuildChallenge(const proto::PublicFileTag& public_tag,
                                  int percent_blocks);

  FileTagSource file_tag_source_;

  std::unique_ptr<ProofSource> proof_source_;
};
}
}
