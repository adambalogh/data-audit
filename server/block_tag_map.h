#pragma once

#include <unordered_map>
#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

// BlockTagMap is a wrapper arount proto::BlockTagMap that lets you extract
// the position of BlockTags conveniently
class BlockTagMap {
 public:
  typedef unsigned long KeyType;
  typedef std::pair<size_t, size_t> ValueType;

  BlockTagMap(const proto::BlockTagMap& block_tag_map)
      : max_size_(block_tag_map.max_size()) {
    size_t previous_block_end = 0;
    for (int i = 0; i < block_tag_map.index_size(); ++i) {
      auto block_index = block_tag_map.index(i);
      auto block_end = block_tag_map.end(i);

      map_[block_index] = std::make_pair(previous_block_end, block_end);
      previous_block_end = block_end;
    }
  }

  size_t MaxSize() const { return max_size_; }

  ValueType FindBlockTag(KeyType index) const {
    try {
      return map_.at(index);
    } catch (std::out_of_range&) {
      throw std::out_of_range(
          std::string{"BlockTag does not exist with index "} +
          std::to_string(index));
    }
  }

 private:
  std::unordered_map<KeyType, ValueType> map_;

  size_t max_size_;
};
}
