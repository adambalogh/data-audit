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

// FileStorage is an interface for storing files with a specified name.
// It may be used to store the user's file, the file containing the BlockTags,
// or any other file we need to store.
//
// Implementations of this interface should be thread-safe and reusable for
// several files.
//
class FileStorage {
 public:
  virtual ~FileStorage() {}

  virtual void StoreFile(const std::string& file_name, std::istream& stream,
                         StorageListener& listener) = 0;
};

// Storage is an interface for storing the File, the File Tag
// and the Block Tags.
//
// All the files and tags should be stored in a way, so that they can be
// retrieved later using a Fetcher.
//
// This class is thread-safe, and can be used for several files
//
class Storage {
 public:
  Storage(std::unique_ptr<FileStorage> file_storage)
      : file_storage_(std::move(file_storage)) {}

  // Stores the given file.
  void StoreFile(const std::string& file_name, std::istream& stream,
                 StorageListener& listener);

  // Stores a PrivateFileTag associated with the given file
  void StoreFileTag(const std::string& file_name,
                    const proto::PrivateFileTag& file_tag,
                    StorageListener& listener);

  // Stores a BlockTag associated with the given file
  void StoreBlockTagFile(const std::string& file_name,
                         const std::string& block_file_path,
                         StorageListener& listener);

  static std::string GetFilesDir() { return "files_dir/"; }

  static std::string GetFilePath(const std::string& file_name) {
    return GetFilesDir() + file_name;
  }

  static std::string GetFileTagPath(const std::string& file_name) {
    return "file_tags/" + file_name;
  }

  static std::string GetBlockTagFilePath(const std::string& file_name) {
    return "block_tags/" + file_name;
  }

 private:
  std::unique_ptr<FileStorage> file_storage_;
};
}
}
