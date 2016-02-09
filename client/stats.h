#pragma once

#include <math.h>
#include <string>

namespace audit {

struct Stats {
  virtual ~Stats() {}

  virtual std::string to_string() const = 0;

 protected:
  // Returns the number of Megabytes the given bytes are equal to, rounded to 3
  // decimal places
  static double ToMB(size_t bytes) {
    return static_cast<double>(floor(bytes / 1000)) / 1000;
  }
};
}
