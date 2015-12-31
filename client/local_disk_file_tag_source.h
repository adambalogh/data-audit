#pragma once

#include "audit/client/file_tag_source.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

class LocalDiskFileTagSource : public FileTagSource {
 public:
  proto::PrivateFileTag GetFileTag(const std::string& file_name) override;
};
}
