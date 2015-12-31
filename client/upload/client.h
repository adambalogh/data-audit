#pragma once

#include <memory>
#include <vector>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/storage.h"

namespace audit {

class Client {
 public:
  Client(std::unique_ptr<Storage> storage) : storage_(std::move(storage)) {}

  void Upload(const File& file);

 private:
  std::unique_ptr<Storage> storage_;
};
}
