#include "audit/providers/local_disk/file_tag_source.h"

#include <fstream>
#include <sstream>

#include "audit/proto/cpor.pb.h"
#include "audit/client/upload/storage.h"
#include "audit/providers/local_disk/file_storage.h"

using audit::upload::Storage;

namespace audit {
namespace providers {
namespace local_disk {

proto::PrivateFileTag FileTagSource::GetFileTag(const std::string& file_name) {
  proto::PrivateFileTag tag;
  std::ifstream tag_file{FileStorage::dir + Storage::GetFileTagPath(file_name),
                         std::ifstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file containing FileTag (" +
                             FileStorage::dir +
                             Storage::GetFileTagPath(file_name) + ")");
  }

  tag.ParseFromIstream(&tag_file);
  return tag;
}
}
}
}
