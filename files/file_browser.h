#pragma once

#include <memory>
#include <string>
#include <vector>

#include "audit/files/file_list_source.h"

namespace audit {
namespace file_browser {

class FileBrowser {
 public:
  FileBrowser(std::unique_ptr<FileListSource> file_list_source)
      : files_(file_list_source->GetFiles()) {}

  // Returns all the files available
  std::vector<File> GetAllFiles();

  // Returns all the files with names that start with the given string
  std::vector<File> Search(const std::string& query);

 private:
  const std::vector<File> files_;
};
}
}
