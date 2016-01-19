#pragma once

#include <vector>
#include <string>

namespace audit {

class FileListSource {
 public:
  virtual std::vector<std::string> GetAllFiles() = 0;

  virtual ~FileListSource() {}
};
}
