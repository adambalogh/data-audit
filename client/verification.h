#pragma once

#include <memory>

#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

class Verification {
 public:
  bool Verify(const proto::PrivateFileTag& file_tag,
              const proto::Challenge& challenge, const proto::Proof& proof,
              std::unique_ptr<PRF> prf);
};
}
