#pragma once

#include "audit/proto/cpor.pb.h"
#include "audit/client/progress_bar.h"
#include "audit/client/upload/file.h"

#include <array>
#include <fstream>
#include <string>

namespace audit {
namespace upload {

// BlockTagSerizer combines all the BlockTags of a file into a single binary
// file.
//
// After we added all the BlockTags, Done() must be called to write all the
// tags to the file.
//
// The location of specific BlockTags can be found in the returned
// proto::BlockTagMap, see it's comments for more explanation.
//
// Once the file containing the BlockTags has been properly stored,
// DeleteTempFile must be called to delete the local copy.
//
class BlockTagSerializer {
 public:
  BlockTagSerializer(const std::string& file_full_path,
                     ProgressBar& progress_bar)
      : file_name_(files_dir + file_full_path),
        out_file_(file_name_, std::ofstream::binary),
        progress_bar_(progress_bar) {
    if (!out_file_) {
      throw std::runtime_error(
          "Could not open file to serialize BlockTags to. (" + file_name_ +
          ")");
    }
  }

  // Each BlockTag of the file must be added
  void Add(const proto::BlockTag& tag);

  // Must be called when we added all the BlockTags
  proto::BlockTagMap Done();

  // Returns the full path and name of the file where the serialized tags are
  // stored
  std::string FileName() const { return file_name_; }

  // Deletes the temporary file where the BlockTags are stored, must be called
  // after we stored the file
  void DeleteTempFile() const;

  // The directory under which all the tag files are stored
  static const std::string files_dir;

 private:
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

  ProgressBar& progress_bar_;
};
}
}
