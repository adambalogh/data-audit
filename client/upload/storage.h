#pragma once

#include <istream>
#include <string>

#include "audit/client/progress_bar.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/stats.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// ReusableStorage is an generic interface for storing the File, the File Tag
// and the Block Tags.
//
// All the files and tags should be stored in a way, so that they can be
// retrieved later using a Fetcher.
//
// Implementations of this interface should be thread safe and they should be
// reusable for several files.
//
class ReusableStorage {
 public:
  virtual void StoreBlockTag(const File& file, const proto::BlockTag& tag) = 0;
  virtual void StoreFileTag(const File& file,
                            const proto::PrivateFileTag& file_tag) = 0;
  virtual void StoreFile(const File& file) = 0;

  virtual ~ReusableStorage() {}
};

// SingleFileStorage should only be used to store a single file and its tags.
class SingleFileStorage {
 public:
  SingleFileStorage(const File& file) : file_(file) {}

  virtual void StoreFile() = 0;
  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag) = 0;
  virtual void StoreBlockTag(const proto::BlockTag& tag) = 0;

  virtual ~SingleFileStorage() {}

 protected:
  const File& file_;
};

class StorageWithProgressBar : public SingleFileStorage {
 public:
  StorageWithProgressBar(const File& file, size_t block_size, int num_blocks,
                         ReusableStorage* reusable_storage)
      : SingleFileStorage(file),
        progress_bar_(CalculateTotalBlocks(file.size, block_size, num_blocks)),
        reusable_storage_(reusable_storage) {}

  void StoreFile() override { reusable_storage_->StoreFile(file_); }

  void StoreFileTag(const proto::PrivateFileTag& file_tag) override {
    reusable_storage_->StoreFileTag(file_, file_tag);
  }

  void StoreBlockTag(const proto::BlockTag& tag) override {
    reusable_storage_->StoreBlockTag(file_, tag);
  }

 private:
  size_t CalculateTotalBlocks(size_t file_size, size_t block_size,
                              int num_blocks) {
    size_t total = static_cast<size_t>(num_blocks) + file_size / block_size;
    if (!file_size % block_size) ++total;
    return total;
  }

  ProgressBar progress_bar_;

  ReusableStorage* const reusable_storage_;
};

// A wrapper class for Storages that provides statistics about the stored
// items.
//
// It should only be used to store a single file and its tags.
//
class StorageWithStats : public SingleFileStorage {
 public:
  StorageWithStats(const File& file, ReusableStorage* reusable_storage)
      : SingleFileStorage(file), reusable_storage_(reusable_storage) {}

  void StoreBlockTag(const proto::BlockTag& tag) override {
    stats_.block_tags_size += tag.ByteSize();
    reusable_storage_->StoreBlockTag(file_, tag);
  }

  void StoreFileTag(const proto::PrivateFileTag& file_tag) override {
    stats_.file_tag_size = file_tag.ByteSize();
    reusable_storage_->StoreFileTag(file_, file_tag);
  }

  void StoreFile() override {
    stats_.file_size = file_.size;
    reusable_storage_->StoreFile(file_);
  }

  // Returns the stats about the storage procedure. It should only be called
  // after all parts of the file have been stored.
  Stats GetStats() const { return stats_; }

 private:
  // The statistics for currently stored file
  Stats stats_;

  // This is the actual storage we are using to store things
  ReusableStorage* const reusable_storage_;
};
}
}
