#include "audit/providers/local_disk/file_storage.h"

#include <array>
#include <iostream>
#include <fstream>
#include <string>

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace providers {
namespace local_disk {

const std::string FileStorage::dir{application_dir};

void FileStorage::StoreFile(const std::string& file_name, std::istream& stream,
                            StorageListener& listener) {
  std::ofstream out_file{dir + file_name, std::ofstream::binary};
  if (!out_file) {
    throw std::runtime_error("Could not open file for writing : " + dir +
                             file_name);
  }

  std::array<char, 1000 * 10> buffer;

  size_t read;
  while (stream.read(buffer.data(), buffer.size()).gcount()) {
    read = stream.gcount();
    out_file.write(buffer.data(), read);
    listener.OnChunkStored(read);
  }
}
}
}
}
