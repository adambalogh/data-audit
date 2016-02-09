#include "audit/providers/local_disk/file_list_source.h"

#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

#include "audit/providers/local_disk/storage.h"

namespace audit {
namespace providers {
namespace local_disk {

using audit::file_browser::File;

std::vector<File> FileListSource::GetFiles() {
  std::vector<File> files;

  auto dir_name = Storage::files_dir;

  DIR *dp;
  struct dirent *ep;
  struct stat file_stats;

  dp = opendir(dir_name.c_str());
  if (dp == NULL) {
    throw std::runtime_error("Unable to open directory containing saved files");
  }

  while ((ep = readdir(dp))) {
    if (ep->d_type != DT_REG) {
      continue;
    }
    stat((dir_name + ep->d_name).c_str(), &file_stats);
    files.push_back(File{ep->d_name, static_cast<size_t>(file_stats.st_size)});
  }
  (void)closedir(dp);

  return files;
}
}
}
}
