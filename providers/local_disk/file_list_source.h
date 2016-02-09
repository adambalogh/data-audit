#pragma once

#include "audit/files/file_list_source.h"

namespace audit {
namespace providers {
namespace local_disk {

class FileListSource : public audit::file_browser::FileListSource {
 public:
  std::vector<audit::file_browser::File> GetFiles();
};
}
}
}
