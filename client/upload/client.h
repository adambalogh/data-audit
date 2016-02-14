#pragma once

#include <memory>
#include <vector>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/progress_bar.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/file.h"
#include "audit/client/upload/storage.h"

namespace audit {
namespace upload {

// This client class should be used to tag and upload files.
class Client {
 public:
  typedef HMACPRF PrfType;

  Client(std::unique_ptr<ReusableStorage> storage)
      : storage_(std::move(storage)) {}

  Stats Upload(const File& file, ProgressBar::CallbackType callback);

 private:
  std::unique_ptr<ReusableStorage> storage_;
};
}
}
