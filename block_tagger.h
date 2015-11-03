#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"

#include "cpor_types.h"
#include "proto/cpor.pb.h"
#include "prf.h"
#include "util.h"

namespace audit {

// BlockTagger returns a BlockTag for each block in the given file.
//
// A block consists of num_sectors sectors, and each sector has a size
// of sector_size bytes. Therefore, the size of a block is equal to
// num_sectors * sector_size bytes. By increasing the block size, we can
// decrease the storage size needed for all the tags, in exchange for
// less accuracy.
//
// The num_blocks field of the passed FileTag will be set to the correct
// value after BlockTagger tagged all the blocks in the file, that is, when
// HasNext() returns false.
//
// Example:
//
//   BlockTagger f{...};
//   while (f.HasNext()) {
//     BlockTag tag = f.GetNext();
//     ...
//   }
//
class BlockTagger {
 public:
  // Constructs a BlockTagger
  //
  // @param file: an istream object whose blocks are going to be tagger.
  //   It must point to the beggining of the stream
  // @param file_tag: a FileTag object, where the num_blocks field doesn't
  //   have to be set. It will be set to the correct value by BlockTagger
  // @param prf: a unique_ptr to a PRF object used for encoding the index of
  //   each block
  //
  BlockTagger(std::istream& file, FileTag* file_tag, std::unique_ptr<PRF> prf)
      : file_(file), file_tag_(file_tag), prf_(std::move(prf)) {
    // Get file size
    file_.seekg(0, file_.end);
    auto length = file_.tellg();
    file_.seekg(0, file_.beg);

    auto block_size = file_tag_->sector_size * file_tag_->num_sectors;

    file_tag->num_blocks = length / block_size;
    if (length % block_size != 0) {
      ++file_tag_->num_blocks;
    }
  }

  // Returns the BlockTag for the next block from the file, should only be
  // called if HasNext() returns true
  proto::BlockTag GetNext();

  // Returns false if we reached the end of the file and there are no more
  // BlockTags to be returned
  bool HasNext() const;

 private:
  bool FillBuffer();

  // Never call this if valid_ is false
  proto::BlockTag GenerateTag();

  // Buffer for reading file
  static const int BUFFER_SIZE = 50 * 1000;  // 0.05 Megabyte
  std::array<unsigned char, BUFFER_SIZE> buffer;

  int start_{0};
  int end_{0};

  unsigned long num_blocks_read_{0};

  bool file_read_{false};

  // The file we are tagging
  std::istream& file_;

  // Pointer to the FileTag of the file being tagged
  FileTag* const file_tag_;

  // Pointer to a pseudorandom function
  std::unique_ptr<PRF> prf_;
};
}
