#pragma once

#include <istream>
#include <string>

#include "audit/client/progress_bar.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/stats.h"
#include "audit/proto/cpor.pb.h"

namespace audit {
namespace upload {

// A listener that can be notified when after we stored a chunk of data.
class StorageListener {
 public:
  virtual void OnChunkStored(size_t bytes) = 0;

  virtual ~StorageListener() {}
};

// A StorageListener that doesn't do anything
class EmptyStorageListener : public StorageListener {
 public:
  void OnChunkStored(size_t bytes) override {}
};

// StorageListenerChain can be used to create a chain of StorageListeners that
// all listen to the same events
class StorageListenerChain : public StorageListener {
 public:
  StorageListenerChain(const std::vector<StorageListener*>& listeners)
      : listeners_(listeners) {}

  void OnChunkStored(size_t bytes) override {
    for (auto listener : listeners_) {
      listener->OnChunkStored(bytes);
    }
  }

 private:
  std::vector<StorageListener*> listeners_;
};

// ProgressBarListener keeps track of the progress of storing the whole file and
// its tags
//
class ProgressBarListener : public StorageListener {
 public:
  ProgressBarListener(ProgressBar& progress_bar)
      : progress_bar_(progress_bar) {}

  void OnChunkStored(size_t bytes) override { progress_bar_.Progress(bytes); }

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
