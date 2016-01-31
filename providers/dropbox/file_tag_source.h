#pragma once

#include "audit/providers/dropbox/dropbox_client.h"
#include "audit/client/verify/file_tag_source.h"

#include "cpprest/uri.h"

namespace audit {
namespace dropbox {

class FileTagSource : verify::FileTagSource, DropboxClient {
 public:
  proto::PrivateFileTag GetFileTag(const std::string& file_name) override;

 private:
  const static web::uri DOWNLOAD_PATH;
};
}
}
