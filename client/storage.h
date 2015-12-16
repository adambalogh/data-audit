#pragma once

#include <istream>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

// Storage is an interface for storing the original File, the File Tag and the
// Block Tags.
//
// All the files and tags should be stored in a way, so that they can be
// retrieved later using a Fetcher.
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
