#include "audit/providers/local_disk/file_storage.h"
#include "gtest/gtest.h"

#include <sstream>
#include <fstream>
#include <string>

#include "audit/client/upload/storage.h"

using namespace audit::providers::local_disk;

using audit::upload::EmptyStorageListener;
using audit::upload::Storage;

TEST(FileStorage, StoreFile) {
  std::string file_name = "gtest_file";
  std::string content = "abcdefgh";
  std::stringstream s{content};
  EmptyStorageListener l;

  FileStorage storage;
  storage.StoreFile(Storage::GetFilePath(file_name), s, l);

  std::ifstream file{FileStorage::dir + Storage::GetFilePath(file_name)};
  EXPECT_TRUE((bool)file);

  std::string in(content.size(), '-');
  file.read(&in[0], in.size());

  EXPECT_EQ(content, in);

  file.read(&in[0], in.size());
  EXPECT_EQ(0, file.gcount()) << "file contains additional data";

  std::remove((FileStorage::dir + Storage::GetFilePath(file_name)).c_str());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
