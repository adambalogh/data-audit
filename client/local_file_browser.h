#pragma once

#include "audit/client/file_browser.h"

namespace audit {

class LocalFileBrowser : public FileBrowser {
 public:
  std::vector<std::string> GetAllFiles();
};
}
