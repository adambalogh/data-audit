#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/file_tag_source.h"

using namespace audit::providers::dropbox;

// IMPORTANT: This test should not be run with Bazel because it doesn't let us
// enter the received code from Dropbox.

TEST(FileTagSource, GetFileTag) {
  TokenSource source;
  source.Initialize([]() {
    std::string code;
    std::cin >> code;
    return code;
  });
  FileTagSource file_tag_source{source};
  std::cout << file_tag_source.GetFileTag("hello").DebugString() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
