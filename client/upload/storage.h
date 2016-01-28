#pragma once

#include <istream>
#include <string>

#include "audit/client/upload/file.h"
#include "audit/client/upload/stats.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// Storage is an interface for storing the original File, the File Tag and the
// Block Tags.
//
// All the files and tags should be stored in a way, so that they can be
// retrieved later using a Fetcher.
//
// Implementations of these classes should be thread safe and they should be
// reusable for several files.
//
class Storage {
 public:
  virtual void StoreBlockTag(const File& file, const proto::BlockTag& tag) = 0;
  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag) = 0;
  virtual void StoreFile(const File& file) = 0;

  virtual ~Storage() {}
};

// A wrapper class for Storages that provides statistics about the stored items.
//
// It should only be used to store a single file and its tags.
//
class StorageWithStats : public Storage {
 public:
  StorageWithStats(Storage* real_storage)
      : real_storage_(std::move(real_storage)) {}

  virtual void StoreBlockTag(const File& file,
                             const proto::BlockTag& tag) override {
    stats_.block_tags_size += tag.ByteSize();
    real_storage_->StoreBlockTag(file, tag);
  }

  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag) override {
    stats_.file_tag_size = file_tag.ByteSize();
    real_storage_->StoreFileTag(file, file_tag);
  }

  virtual void StoreFile(const File& file) override {
    stats_.file_size = file.size;
    real_storage_->StoreFile(file);
  }

  // Returns the stats about the storage procedure. It should only be called
  // after all parts of the file have been stored.
  Stats GetStats() const { return stats_; }

 private:
  // The statistics for currently stored file
  Stats stats_;

  // This is the actual storage we are using to store things
  Storage* const real_storage_;
};
}
}
