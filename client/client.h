#pragma once

#include <memory>
#include <vector>

#include "openssl/bn.h"

#include "audit/util.h"
#include "audit/client/block_tagger.h"
#include "audit/client/file.h"
#include "audit/client/storage.h"

namespace audit {

class Client {
 public:
  Client(std::unique_ptr<Storage> storage,
         std::unique_ptr<RandomNumberGenerator> random_gen,
         std::unique_ptr<PRF> prf)
      : storage_(std::move(storage)),
        random_gen_(std::move(random_gen)),
        prf_(std::move(prf)) {}

  void Upload(std::istream& file, const std::string& file_name);

 private:
  std::unique_ptr<Storage> storage_;
  std::unique_ptr<RandomNumberGenerator> random_gen_;
  std::unique_ptr<PRF> prf_;
};
}
