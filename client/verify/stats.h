#pragma once

#include <string>
#include <sstream>

namespace audit {
namespace verify {

struct Stats {
  Stats() {}

  Stats(int proof_size) : proof_size(proof_size) {}

  int proof_size;

  std::string to_string() const {
    std::stringstream repr;
    repr << "Proof size: ";
    repr << proof_size;
    repr << " bytes";
    return repr.str();
  }
};
}
}
