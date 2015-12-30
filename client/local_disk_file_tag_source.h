#pragma once

#include "audit/client/file_tag_source.h"

namespace audit {

class LocalDiskFileTagSource : public FileTagSource {
 public:
  proto::PrivateFileTag GetFileTag(const std::string& file_name) override {
    proto::PrivateFileTag tag;
    std::ifstream tag_file_;
    std::string file_dir_{"/users/adambalogh/Developer/audit/files_dir/"};
    tag_file_.open(file_dir_ + "file_Tag" + file_name);

    std::stringstream buffer;
    buffer << tag_file_.rdbuf();
    tag.ParseFromString(buffer.str());
    return tag;
  }
};
}
