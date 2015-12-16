#pragma once

#include <istream>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

class Storage {
 public:
  virtual void StoreBlockTag(const proto::BlockTag& tag) = 0;
  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag) = 0;
  virtual void StoreFile(std::istream& file) = 0;

  virtual ~Storage() {}
};

class StorageUsingFileName : public Storage {
 public:
  StorageUsingFileName(const std::string& file_name) : file_name_(file_name) {}

 protected:
  const std::string file_name_;
};
}
