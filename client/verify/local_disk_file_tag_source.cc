#include "audit/client/verify/local_disk_file_tag_source.h"

#include <fstream>
#include <sstream>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace verify {

proto::PrivateFileTag LocalDiskFileTagSource::GetFileTag(
    const std::string& file_name) {
  proto::PrivateFileTag tag;
  std::ifstream tag_file_;
  std::string file_dir_{"/users/adambalogh/Developer/audit/files_dir/"};
  tag_file_.open(file_dir_ + "file_Tag" + file_name);

  std::stringstream buffer;
  buffer << tag_file_.rdbuf();
  tag.ParseFromString(buffer.str());
  return tag;
}
}
}
