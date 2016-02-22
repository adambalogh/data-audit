#pragma once

#include <string>
#include <vector>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

class BinaryFileTagSource {
 public:
  virtual ~BinaryFileTagSource() {}

  virtual std::vector<uint8_t> GetFileTag(const std::string& file_name) = 0;
};

// FileTagSource should download the PrivateFileTag of the requested file from
// the cloud service provider, so that we can construct a Challenge for the
// file using the information from the tag, and send it to the CSP.
//
class FileTagSource {
 public:
  FileTagSource(std::unique_ptr<BinaryFileTagSource> source)
      : source_(std::move(source)) {}

  proto::PrivateFileTag GetFileTag(const std::string& file_name);

 private:
  std::unique_ptr<BinaryFileTagSource> source_;
};
}
}
