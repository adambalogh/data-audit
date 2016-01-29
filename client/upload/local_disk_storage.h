#pragma once

#include <string>

#include "audit/client/upload/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// Storage implementation that stores all the data on the local disk.
//
// It is only created for testing purposes.
class LocalDiskStorage : public ReusableStorage {
 public:
  void StoreFile(const std::string& file_name, std::istream& stream,
                 StorageListener& listener) override;

  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    StorageListener& listener) override;

  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         StorageListener& listener) override;

  // Returns the full path to the file where the given file is stored
  static std::string GetFilePath(const std::string& file_name) {
    return files_dir + file_name;
  }

  // Returns the full path to the file where the PrivateFileTag of the given
  // file is stored
  static std::string GetFileTagPath(const std::string& file_name) {
    return tags_dir + "file_tag_" + file_name;
  }

  // Returns the full path to the fule where all the BlockTags are stored for
  // the given file
  static std::string GetBlockTagFilePath(const std::string& file_name) {
    return tags_dir + "tags_file_" + file_name;
  }

  // This is where all the files are stored on the local disk
  static const std::string files_dir;

  // This is a separate sub-directory for storing all the tags
  static const std::string tags_dir;
};
}
}
