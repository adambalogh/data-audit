#include "audit/client/verify/local_disk_file_tag_source.h"

#include <fstream>
#include <sstream>

#include "audit/proto/cpor.pb.h"
#include "audit/client/upload/local_disk_storage.h"

namespace audit {
namespace verify {

proto::PrivateFileTag LocalDiskFileTagSource::GetFileTag(
    const std::string& file_name) {
  proto::PrivateFileTag tag;
  std::ifstream tag_file_;
  tag_file_.open(upload::LocalDiskStorage::GetFileTagPath(file_name));

  std::stringstream buffer;
  buffer << tag_file_.rdbuf();
  tag.ParseFromString(buffer.str());
  return tag;
}
}
}
