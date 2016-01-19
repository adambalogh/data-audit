#pragma once

#include "audit/files/file_list_source.h"

namespace audit {

class LocalFileListSource : public FileListSource {
 public:
  std::vector<std::string> GetAllFiles();
};
}
