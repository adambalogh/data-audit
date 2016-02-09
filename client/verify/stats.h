#pragma once

#include <string>
#include <sstream>

#include "audit/client/stats.h"

namespace audit {
namespace verify {

struct Stats : public ::audit::Stats {
  Stats() {}

  Stats(size_t proof_size) : proof_size(proof_size) {}

  std::string to_string() const {
    std::stringstream repr;
    repr << "Proof size: " << ToMB(proof_size) << " MB" << std::endl;
    return repr.str();
  }

  size_t proof_size;
};
}
}
