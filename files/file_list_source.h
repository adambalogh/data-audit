#pragma once

#include <iomanip>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>

namespace audit {
namespace file_browser {

struct File {
  File(const std::string& name, size_t size) : name(name), size(size) {}

  std::string Size() const {
    std::stringstream out;
    out << std::fixed << std::setprecision(1);

    // We assume all the files will be at least 1 KB
    auto kilobytes = size / 1000.0;
    if (kilobytes < 1000) {
      out << kilobytes << " KB";
    } else if (kilobytes / 1000.0 < 1000) {
      out << kilobytes / 1000.0 << " MB";
    } else {
      out << kilobytes / 1000000.0 << " GB";
    }
    return out.str();
  }

  const std::string name;
  const size_t size;
};

class FileListSource {
 public:
  virtual std::vector<File> GetFiles() = 0;

  virtual ~FileListSource() {}
};
}
}
