#include "audit/files/file_browser.h"

namespace audit {
namespace file_browser {

std::vector<File> FileBrowser::GetAllFiles() { return files_; }

std::vector<File> FileBrowser::Search(const std::string& query) {
  std::vector<File> result;
  for (const auto& file : files_) {
    if (file.name.compare(0, query.size(), query) == 0) {
      result.push_back(file);
    }
  }
  return result;
}
}
}
