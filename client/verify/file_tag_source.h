#pragma once

#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

class FileTagSource {
 public:
  virtual proto::PrivateFileTag GetFileTag(const std::string& file_name) = 0;
};
}
