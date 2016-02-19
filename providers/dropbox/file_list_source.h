#pragma once

#include "audit/files/file_list_source.h"
#include "audit/providers/dropbox/dropbox_client.h"
#include "audit/providers/dropbox/dropbox_urls.h"

namespace audit {
namespace providers {
namespace dropbox {

class FileListSource : public DropboxClient,
                       public file_browser::FileListSource {
 public:
  FileListSource(TokenSource& token_source)
      : DropboxClient(token_source, AUTH_URL) {}

  std::vector<file_browser::File> GetFiles() override;
};
}
}
}
