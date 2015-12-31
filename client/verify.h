#pragma once

#include <memory>

#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

// Verifies a file's integrity based on the proof recieved from the server.
// Returns true if the file is intact.
bool Verify(const proto::PrivateFileTag& file_tag,
            const proto::Challenge& challenge, const proto::Proof& proof,
            std::unique_ptr<PRF> prf);
}
