#include "gtest/gtest.h"

#include "audit/providers/dropbox/storage.h"

#include "audit/client/upload/storage.h"

#include <sstream>

using namespace audit::providers::dropbox;

class TokenSourceStub : public TokenSourceInterface {
 public:
  std::string GetToken() { return "a"; }
};

TEST(Storage, StoreFile) {
  TokenSourceStub token_source;
  Storage storage{token_source};

  audit::upload::EmptyStorageListener listener;

  std::ifstream file{"/Users/adambalogh/Desktop/examulator.numbers"};
  if (!file) {
    throw std::runtime_error{"yo"};
  }

  storage.StoreFile("large_file", file, listener);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
