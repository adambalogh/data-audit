#pragma once

#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

// FileTagSource should download the PrivateFileTag of the requested file from
// the cloud service provider, so that we can construct a Challenge for the
// file using the information from the tag, and send it to the CSP.
//
class FileTagSource {
 public:
  virtual proto::PrivateFileTag GetFileTag(const std::string& file_name) = 0;

  virtual ~FileTagSource() {}
};
}
}
