#pragma once

#include <string>

#include "audit/client/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

// Storage implementation that stores all the data on the local disk.
//
// It is only created for testing purposes.
class LocalDiskStorage : public StorageUsingFileName {
 public:
  LocalDiskStorage(const std::string& file_name)
      : StorageUsingFileName(file_name) {}

  virtual void StoreBlockTag(const proto::BlockTag& tag) override;
  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag) override;
  virtual void StoreFile(std::istream& file) override;

 private:
  // Everything is stored under this directiry
  std::string file_dir_{"/users/adambalogh/Developer/audit/files_dir/"};
};
}
