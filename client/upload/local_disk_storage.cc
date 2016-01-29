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

void LocalDiskStorage::StoreBlockTagFile(const std::string& file_name,
                                         const std::string& block_file_path,
                                         StorageListener& listener) {
  std::ofstream out_file{GetBlockTagFilePath(file_name), std::ofstream::binary};
  if (!out_file) {
    throw std::runtime_error("Could not open file to write BlockTag (" +
                             GetBlockTagFilePath(file_name) + ")");
  }

  std::ifstream input{block_file_path, std::ifstream::binary};
  if (!input) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }

  std::array<char, 1000 * 10> buffer;
  size_t written;
  while (input.read(buffer.data(), buffer.size()).gcount()) {
    written = input.gcount();
    out_file.write(buffer.data(), written);
    listener.OnBlockTagFileChunkStored(written);
  }
}

void LocalDiskStorage::StoreFileTag(const std::string& file_name,
                                    const proto::PrivateFileTag& file_tag,
                                    StorageListener& listener) {
  std::ofstream tag_file{GetFileTagPath(file_name), std::ofstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file to write FileTag (" +
                             GetFileTagPath(file_name) + ")");
  }
  file_tag.SerializeToOstream(&tag_file);
  listener.OnFileTagStored(file_tag.ByteSize());
}

void LocalDiskStorage::StoreFile(const std::string& file_name,
                                 std::istream& stream,
                                 StorageListener& listener) {
  std::ofstream out_file{GetFilePath(file_name), std::ofstream::binary};
  if (!out_file) {
    throw std::runtime_error(
        "Could not open file to write the original file (" +
        GetFilePath(file_name) + ")");
  }
  std::array<char, 1000 * 10> buffer;
  size_t written;
  while (stream.read(buffer.data(), buffer.size()).gcount()) {
    written = stream.gcount();
    out_file.write(buffer.data(), written);
    listener.OnFileChunkStored(written);
  }
}
}
}
