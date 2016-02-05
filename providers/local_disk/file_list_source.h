#pragma once

#include "audit/files/file_list_source.h"

namespace audit {
namespace providers {
namespace local_disk {

class FileListSource : public audit::FileListSource {
 public:
  std::vector<const std::string> GetFiles();
};
}
}
}
