#include "gtest/gtest.h"

#include "audit/providers/dropbox/dropbox_storage.h"

#include "audit/client/upload/storage.h"

#include <sstream>

using namespace audit::upload;

TEST(DropboxStorage, StoreFile) {
  DropboxStorage storage{
      "dTCrjxub6z0AAAAAAAACkyOSk147wf27A5j5RAykOl1vyDNTWQ18Oy6gZKiAlcJp"};

  EmptyStorageListener listener;

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
