#pragma once

#include "audit/proto/cpor.pb.h"

namespace audit {

// BlockTagMap is a wrapper arount proto::BlockTagMap that lets you extract
// the position of BlockTags conveniently
class BlockTagMap {
 public:
  BlockTagMap(const proto::BlockTagMap& map) : map_(map) {}

  std::pair<size_t, size_t> FindBlockTag(unsigned long index) {
    auto iter = std::find(map_.index().begin(), map_.index().end(), index);
    if (iter == map_.index().end()) {
      throw std::range_error("BlockTag does not exist with given index: " +
                             std::to_string(index));
    }
    auto key = std::distance(map_.index().begin(), iter);

    if (key == 0) {
      return std::make_pair(0, map_.end(key));
    }
    return std::make_pair(map_.end(key - 1), map_.end(key));
  }

 private:
  const proto::BlockTagMap map_;
};
}
