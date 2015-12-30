#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/file.h"
#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

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
// value.
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
  // Constructs a BlockTagger.
  //
  // @param file: The file's content will be tagged. This object must outlive
  //   the BlockTagger.
  // @param prf: A PRF that will be used for making the tags secure.
  //
  BlockTagger(const File& file, std::unique_ptr<PRF> prf)
      : file_(file), prf_(std::move(prf)) {
    buffer.resize(
        std::max(file_.sector_size(), static_cast<size_t>(1000) * 1000));
  }

  // Returns the BlockTag for the next block from the file, should only be
  // called if HasNext() returns true
  proto::BlockTag GetNext();

  // Returns false if we reached the end of the file and there are no more
  // BlockTags to be returned
  bool HasNext() const;

 private:
  void FillBuffer();

  proto::BlockTag GenerateTag();

  // Buffer for reading file
  std::vector<unsigned char> buffer;

  int start_{0};
  int end_{0};

  unsigned long num_blocks_read_{0};

  bool file_read_{false};

  // Pointer to the FileTag of the file being tagged
  const File& file_;

  // Pointer to a pseudorandom function
  std::unique_ptr<PRF> prf_;

  // BN context used for computations
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};
};
}
