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

std::vector<uint8_t> FileTagSource::GetFileTag(const std::string& file_name) {
  std::ifstream tag_file{FileStorage::dir + Storage::GetFileTagPath(file_name),
                         std::ifstream::binary};
  if (!tag_file) {
    throw std::runtime_error("Could not open file containing FileTag (" +
                             FileStorage::dir +
                             Storage::GetFileTagPath(file_name) + ")");
  }

  std::vector<uint8_t> bin;
  tag_file.seekg(0, std::ios_base::end);
  auto size = tag_file.tellg();
  tag_file.seekg(0, std::ios_base::beg);
  bin.resize(size);
  tag_file.read(reinterpret_cast<char*>(&bin[0]), bin.size());
  return bin;
}
}
}
}
