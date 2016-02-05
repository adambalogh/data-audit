#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/file_list_source.h"

using namespace audit::providers::dropbox;

TEST(FileListSource, GetFiles) {
  TokenSource source;
  source.Initialize([]() {
    std::string code;
    std::cin >> code;
    return code;
  });

  FileListSource s{source};
  for (auto& file : s.GetFiles()) std::cout << file << std::endl;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
