#include "audit/files/file_browser.h"

namespace audit {

std::vector<const std::string> FileBrowser::GetAllFiles() { return files_; }

std::vector<const std::string> FileBrowser::Search(const std::string& query) {
  std::vector<const std::string> result;
  for (const auto& file : files_) {
    if (file.compare(0, query.size(), query) == 0) {
      result.push_back(file);
    }
  }
  return result;
}
}
