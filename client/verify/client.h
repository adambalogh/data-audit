#pragma once

#include <string>

#include "audit/client/verify/file_tag_source.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

class Client {
 public:
  Client(std::unique_ptr<FileTagSource> file_tag_source)
      : file_tag_source_(std::move(file_tag_source)) {}

  bool Verify(const std::string& file_full_path, int percent_blocks);

 private:
  proto::Challenge BuildChallenge(const proto::PublicFileTag& public_tag,
                                  int percent_blocks);

  proto::Proof GetProof(proto::Challenge challenge);

  std::unique_ptr<FileTagSource> file_tag_source_;
};
}
}
