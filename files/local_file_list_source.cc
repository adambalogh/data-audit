#include "audit/files/local_file_list_source.h"

#include <iostream>

#include <dirent.h>

#include "audit/providers/local_disk/local_disk_storage.h"

namespace audit {

std::vector<const std::string> LocalFileListSource::GetFiles() {
  std::vector<const std::string> files;

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
