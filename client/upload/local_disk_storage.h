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
  virtual void StoreFile(const File& file, StorageListener& listener) override;

  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag,
                            StorageListener& listener) override;

  virtual void StoreBlockTagFile(const File& file, const std::string& file_name,
                                 StorageListener& listener) override;
  // This is where all the files are stored on the local disk
  static const std::string files_dir;

  // This is a separate sub-directory for storing all the tags
  static const std::string tags_dir;
};
}
}
