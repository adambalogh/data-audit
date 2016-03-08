#pragma once

#include "audit/proto/cpor.pb.h"
#include "audit/client/upload/progress_bar.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/progressable.h"

#include <array>
#include <fstream>
#include <string>

namespace audit {
namespace upload {

// BlockTagSerizer combines all the BlockTags of a file into a single binary
// file.
//
// After we added all the BlockTags, Done() must be called to make sure all the
// tags are written to the file.
//
// Once Done() has returned, the temporary file located at FileName() will
// contain all the added tags. It must saved to the server before the
// BlockTagSerializer object is destroyed, as it will automatically remove this
// temporary file.
//
// The location of specific BlockTags can be found in the returned
// proto::BlockTagMap, see it's comments for more explanation.
//
// Example:
//
//    BlockTagSerializer s{...};
//    for (BlockTag& tag : tags) {
//      s.Add(tag);
//    }
//    s.Done();
//    StoreFile(s.FileName(), ...);
//
class BlockTagSerializer final : private Progressable {
 public:
  BlockTagSerializer(const std::string& file_name, ProgressBar& progress_bar)
      : Progressable(progress_bar),
        file_name_(files_dir + file_name),
        out_file_(file_name_, std::ofstream::binary) {
    if (!out_file_) {
      throw std::runtime_error(
          "Could not open file to serialize BlockTags to. (" + file_name_ +
          ")");
    }
    block_tag_map_.set_max_size(0);
  }

  ~BlockTagSerializer() { DeleteTempFile(); }

  // Each BlockTag of the file must be added
  void Add(const proto::BlockTag& tag);

  // Must be called when we added all the BlockTags
  //
  // It returns a BlockTagMap containing all the information needed to retrieve
  // individual BlockTags later
  proto::BlockTagMap Done();

  // Returns the full path and name of the file where the serialized tags are
  // stored
  std::string FilePath() const { return file_name_; }

  // The directory under which all the tag files are stored
  static const std::string files_dir;

 private:
  // Deletes the temporary file where the BlockTags are stored
  void DeleteTempFile() const;

  // Writes the content of the buffer to out_file
  void Flush();

  // Name of the file where the tags are serialized to
  const std::string file_name_;

  // The file where we serialize all the BlockTags
  std::ofstream out_file_;

  // Buffer for tags
  std::vector<proto::BlockTag> tags_;

  // The size of tags_ in bytes
  size_t buffer_size_{0};

  // Indicates the current size of out_file
  size_t file_end_{0};

  proto::BlockTagMap block_tag_map_;
};
}
}
