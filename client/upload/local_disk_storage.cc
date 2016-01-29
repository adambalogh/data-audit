#include "audit/client/upload/local_disk_storage.h"

#include <array>
#include <iostream>
#include <fstream>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

const std::string LocalDiskStorage::files_dir{
    "/users/adambalogh/Developer/audit/files_dir/"};

const std::string LocalDiskStorage::tags_dir{files_dir + "tags/"};

void LocalDiskStorage::StoreBlockTagFile(const File& file,
                                         const std::string& file_name,
                                         StorageListener& listener) {
  std::ofstream out_file{tags_dir + "tags_file_" + file.file_name,
                         std::ofstream::binary};
  if (!out_file) {
    throw std::runtime_error("Could not open file to write BlockTag");
  }

  std::ifstream input{file_name, std::ifstream::binary};
  if (!input) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             file_name + ")");
  }

  std::array<char, 1000> buffer;
  while (input.read(buffer.data(), buffer.size()).gcount()) {
    out_file.write(buffer.data(), input.gcount());
  }
}

void LocalDiskStorage::StoreFileTag(const File& file,
                                    const proto::PrivateFileTag& file_tag,
                                    StorageListener& listener) {
  std::ofstream tag_file{tags_dir + "file_tag_" + file.file_name,
                         std::ofstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file to write FileTag");
  }
  file_tag.SerializeToOstream(&tag_file);
}

void LocalDiskStorage::StoreFile(const File& file, StorageListener& listener) {
  std::ofstream out_file{files_dir + file.file_name, std::ofstream::binary};
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
