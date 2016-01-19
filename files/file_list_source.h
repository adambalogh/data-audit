#pragma once

#include <vector>
#include <string>

namespace audit {

class FileListSource {
 public:
  virtual std::vector<const std::string> GetFiles() = 0;

  virtual ~FileListSource() {}
};
}
