#pragma once

#include <istream>

#include "audit/client/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

class AmazonS3Storage : public Storage {
 public:
  virtual void StoreBlockTag(const proto::BlockTag& tag);
  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag);
  virtual void StoreFile(std::istream& file);
};
}
