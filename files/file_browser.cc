#include "audit/files/file_browser.h"

namespace audit {

std::vector<const std::string> FileBrowser::GetAllFiles() { return files_; }

std::vector<const std::string> FileBrowser::Search(const std::string& query) {
  std::vector<const std::string> result;
  std::copy_if(std::begin(files_), std::begin(files_),
               std::back_inserter(result),
               [query](const std::string& file_name) { return true; });
  return result;
}
}
