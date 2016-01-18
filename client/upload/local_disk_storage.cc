#include "audit/client/upload/local_disk_storage.h"

#include <array>
#include <iostream>
#include <fstream>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

const std::string LocalDiskStorage::file_dir_{
    "/users/adambalogh/Developer/audit/files_dir/"};

void LocalDiskStorage::StoreBlockTag(const File& file,
                                     const proto::BlockTag& tag) {
  std::ofstream tag_file{
      file_dir_ + "tags" + file.file_full_path + std::to_string(tag.index()),
      std::ofstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file to write BlockTag");
  }
  tag.SerializeToOstream(&tag_file);
}

void LocalDiskStorage::StoreFileTag(const File& file,
                                    const proto::PrivateFileTag& file_tag) {
  std::ofstream tag_file{file_dir_ + "file_tag" + file.file_full_path,
                         std::ofstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file to write FileTag");
  }
  file_tag.SerializeToOstream(&tag_file);
}

void LocalDiskStorage::StoreFile(const File& file) {
  std::ofstream out_file{file_dir_ + file.file_full_path,
                         std::ofstream::binary};
  if (!out_file) {
    throw std::runtime_error("Could not open file to write the original file");
  }
  std::array<char, 1000> buffer;

  while (file.stream.read(buffer.data(), buffer.size()).gcount()) {
    out_file.write(buffer.data(), file.stream.gcount());
  }
}
}
}
