#pragma once

#include "audit/client/verify/file_tag_source.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

class LocalDiskFileTagSource : public FileTagSource {
 public:
  proto::PrivateFileTag GetFileTag(const std::string& file_name) override;
};
}
}
