#pragma once

#include <istream>
#include <string>

#include "audit/client/upload/file.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// Storage is an interface for storing the original File, the File Tag and the
// Block Tags.
//
// All the files and tags should be stored in a way, so that they can be
// retrieved later using a Fetcher.
class Storage {
 public:
  virtual void StoreBlockTag(const File& file, const proto::BlockTag& tag) = 0;
  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag) = 0;
  virtual void StoreFile(const File& file) = 0;

  virtual ~Storage() {}
};

// class StorageWithCheck : public Storage {
//  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag) override {
//    if (!(file_tag.alphas_size() && file_tag.prf_key().size() &&
//          file_tag.has_public_tag() && file_tag.public_tag().num_blocks() &&
//          file_tag.public_tag().num_sectors() &&
//          file_tag.public_tag().sector_size() &&
//          file_tag.public_tag().p().size() &&
//          file_tag.public_tag().file_name().size())) {
//      throw std::logic_error("FileTag is missing information");
//    }
//  }
//};
}
}
