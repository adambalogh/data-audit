#pragma once

#include <string>

#include "audit/client/upload/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace providers {
namespace local_disk {

class FileStorage : public upload::FileStorage {
 public:
  typedef upload::StorageListener StorageListener;

  void StoreFile(const std::string& name, std::istream& stream,
                 StorageListener& listener) override;

  static const std::string dir;
};
}
}
}
