#include "audit/providers/local_disk/local_disk_file_tag_source.h"

#include <fstream>
#include <sstream>

#include "audit/proto/cpor.pb.h"
#include "audit/providers/local_disk/local_disk_storage.h"

namespace audit {
namespace verify {

proto::PrivateFileTag LocalDiskFileTagSource::GetFileTag(
    const std::string& file_name) {
  proto::PrivateFileTag tag;
  std::ifstream tag_file{upload::LocalDiskStorage::GetFileTagPath(file_name),
                         std::ifstream::binary};
  if (!tag_file) {
    throw std::runtime_error(
        "Could not open file containing FileTag (" +
        upload::LocalDiskStorage::GetFileTagPath(file_name) + ")");
  }

  tag.ParseFromIstream(&tag_file);
  return tag;
}
}
}
