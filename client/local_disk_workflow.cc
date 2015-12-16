#include "audit/client/local_disk_workflow.h"

#include <string>
#include <fstream>

#include "audit/proto/cpor.pb.h"

namespace audit {

void LocalDiskWorkFlow::ProcessBlockTag(const proto::BlockTag& tag) {
  std::ofstream tag_file{file_dir_ + "tags" + file_name_ +
                         std::to_string(tag.index())};
  tag.SerializeToOstream(&tag_file);
}

void LocalDiskWorkFlow::Finish(const proto::PrivateFileTag& file_tag) {
  std::ofstream tag_file{file_dir_ + "file_tag" + file_name_};
  file_tag.SerializeToOstream(&tag_file);
}
}
