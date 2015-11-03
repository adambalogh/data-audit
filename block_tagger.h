#pragma once

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
// Example usage:
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
  //   It must point to the beggining of the stream.
  // @param file_tag: a FileTag object, where the num_blocks field doesn't
  //   have to be set. It will be set to the correct value by BlockTagger.
  // @param prf: a unique_ptr to a PRF object
  BlockTagger(std::istream& file, FileTag* file_tag, std::unique_ptr<PRF> prf)
      : file_(file), file_tag_(file_tag), prf_(std::move(prf)) {
    CheckValid();
    file_tag->num_blocks = 0;
  }

  // Returns the BlockTag for the next block from the file, should only be
  // called if HasNext() returns true
  proto::BlockTag GetNext();

  // Returns false if we reached the end of the file and there are no more
  // BlockTags to be returned
  bool HasNext() const;

 private:
  static const sector_size = 16;

  void CheckValid();

  // Never call this if valid_ is false
  proto::BlockTag GenerateTag();

  // The file we are tagging
  std::istream& file_;

  FileTag* const file_tag_;

  std::unique_ptr<PRF> prf_;

  // Indicates whether we can read more from the file
  bool valid_{true};
};
}
