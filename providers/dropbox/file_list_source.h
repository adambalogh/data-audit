#pragma once

#include "audit/files/file_list_source.h"
#include "audit/providers/dropbox/dropbox_client.h"
#include "audit/providers/dropbox/dropbox_urls.h"

namespace audit {
namespace dropbox {

class FileListSource : public DropboxClient, public audit::FileListSource {
 public:
  FileListSource(TokenSource& token_source)
      : DropboxClient(token_source, AUTH_URL) {}

  std::vector<const std::string> GetFiles() override;
};
}
}
