#pragma once

#include <istream>
#include <string>

#include "audit/client/progress_bar.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/stats.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// A handler that can be notified when after we stored a chunk of data using a
// ReusableStorage. It is used for the progress bar.
class StorageListener {
 public:
  // Should be called whenever a chunk of the file has been stored
  virtual void OnFileChunkStored(size_t bytes) = 0;

  // Should be called whenever one or more block tags has been stored
  virtual void OnBlockTagFileChunkStored(size_t bytes) = 0;

  // Should be called when the file tag has been stored
  virtual void OnFileTagStored(size_t bytes) = 0;
};

// A StorageListener that doesn't do anything
class EmptyStorageListener : public StorageListener {
 public:
  void OnFileChunkStored(size_t bytes) override {}
  void OnBlockTagFileChunkStored(size_t bytes) override {}
  void OnFileTagStored(size_t bytes) override {}
};

// StorageListenerChain can be used to create a chain of StorageListeners that
// all listen to the same events
class StorageListenerChain : public StorageListener {
 public:
  StorageListenerChain(const std::vector<StorageListener*>& listeners)
      : listeners_(listeners) {}

  void OnFileChunkStored(size_t bytes) override {
    for (auto listener : listeners_) {
      listener->OnFileChunkStored(bytes);
    }
  }

  void OnBlockTagFileChunkStored(size_t bytes) override {
    for (auto listener : listeners_) {
      listener->OnBlockTagFileChunkStored(bytes);
    }
  }

  void OnFileTagStored(size_t bytes) override {
    for (auto listener : listeners_) {
      listener->OnFileTagStored(bytes);
    }
  }

 private:
  std::vector<StorageListener*> listeners_;
};

// StatsListener keeps track of how much storage is used for a particular file
// and its tags
//
class StatsListener : public StorageListener {
 public:
  void OnBlockTagFileChunkStored(size_t bytes) override {
    stats_.block_tags_size += bytes;
  }

  void OnFileTagStored(size_t bytes) override { stats_.file_tag_size += bytes; }

  void OnFileChunkStored(size_t bytes) override { stats_.file_size += bytes; }

  // Returns the stats about the storage procedure. It should only be called
  // after all parts of the file have been stored.
  Stats GetStats() const { return stats_; }

 private:
  // Stats for the the current file and its tags
  Stats stats_;
};

// ProgressBarListener keeps track of the progress of storing the whole file and
// its tags
//
class ProgressBarListener : public StorageListener {
 public:
  ProgressBarListener(ProgressBar& progress_bar)
      : progress_bar_(progress_bar) {}

  void OnFileChunkStored(size_t bytes) override {
    progress_bar_.Progress(bytes);
  }
  void OnBlockTagFileChunkStored(size_t bytes) override {}

  void OnFileTagStored(size_t bytes) override { progress_bar_.Progress(bytes); }

 private:
  ProgressBar& progress_bar_;
};

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
  // Stores the given file.
  virtual void StoreFile(const std::string& file_name, std::istream& stream,
                         StorageListener& listener) = 0;

  // Stores a PrivateFileTag associated with the given file
  virtual void StoreFileTag(const std::string& file_name,
                            const proto::PrivateFileTag& file_tag,
                            StorageListener& listener) = 0;

  // Stores a BlockTag associated with the given file
  virtual void StoreBlockTagFile(const std::string& file_name,
                                 const std::string& block_file_path,
                                 StorageListener& listener) = 0;

  virtual ~ReusableStorage() {}
};
}
}
