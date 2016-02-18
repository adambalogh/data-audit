#pragma once

#include <string>
#include <sstream>

#include "audit/client/stats.h"

namespace audit {
namespace verify {

struct Stats : public ::audit::Stats {
  Stats() {}

  Stats(size_t file_tag_size, size_t challenge_size, size_t proof_size)
      : file_tag_size(file_tag_size),
        challenge_size(challenge_size),
        proof_size(proof_size) {}

  std::string to_string() const {
    std::stringstream repr;
    repr << "FileTag size: " << ToMB(file_tag_size) << " MB" << std::endl;
    repr << "Challenge size: " << ToMB(challenge_size) << " MB" << std::endl;
    repr << "Proof size: " << ToMB(proof_size) << " MB" << std::endl;
    return repr.str();
  }

  size_t file_tag_size;
  size_t challenge_size;
  size_t proof_size;
};
}
}
