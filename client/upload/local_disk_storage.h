#pragma once

#include <string>

#include "audit/client/upload/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// Storage implementation that stores all the data on the local disk.
//
// It is only created for testing purposes.
class LocalDiskStorage : public Storage {
 public:
  virtual void StoreBlockTag(const File& file,
                             const proto::BlockTag& tag) override;
  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag) override;
  virtual void StoreFile(const File& file) override;

  // Everything is stored under this directory
  static const std::string files_dir;
  static const std::string tags_dir;
};
}
}
