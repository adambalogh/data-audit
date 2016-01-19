#include "audit/files/local_file_list_source.h"

#include <iostream>

#include <dirent.h>

#include "audit/client/upload/local_disk_storage.h"

namespace audit {

std::vector<std::string> LocalFileListSource::GetAllFiles() {
  std::vector<std::string> files;

  auto dir_name = upload::LocalDiskStorage::files_dir;

  DIR *dp;
  struct dirent *ep;

  dp = opendir(dir_name.c_str());
  if (dp == NULL) {
    // error
  }

  while ((ep = readdir(dp))) {
    if (ep->d_type != DT_REG) {
      continue;
    }
    files.push_back(ep->d_name);
  }
  (void)closedir(dp);

  return files;
}
}
