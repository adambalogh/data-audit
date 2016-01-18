#pragma once

#include <vector>
#include <string>

namespace audit {

class FileBrowser {
 public:
  virtual std::vector<std::string> GetAllFiles() = 0;

  virtual ~FileBrowser() {}
};
}
