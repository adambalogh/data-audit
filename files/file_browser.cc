#include "audit/files/file_browser.h"

namespace audit {
namespace file_browser {

std::vector<File> FileBrowser::GetAllFiles() { return files_; }

std::vector<File> FileBrowser::Search(std::string query) {
  std::transform(query.begin(), query.end(), query.begin(), ::tolower);
  std::vector<File> result;
  for (const auto& file : files_) {
    std::string lower_case = file.name;
    std::transform(lower_case.begin(), lower_case.end(), lower_case.begin(),
                   ::tolower);
    if (lower_case.compare(0, query.size(), query) == 0) {
      result.push_back(file);
    }
  }
  return result;
}
}
}
