#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_tag_source.h"

#include <iostream>

using namespace audit::dropbox;

// IMPORTANT: This test should not be run with Bazel because it doesn't let us
// enter the received code from Dropbox.

TEST(Fetcher, GetBlock) {
  TokenSource source{[]() {
    std::string code;
    std::cin >> code;
    return code;
  }};
  FileTagSource file_tag_source{source};
  auto file_tag = file_tag_source.GetFileTag("hello");

  Fetcher fetcher(source);
  std::cout << fetcher.FetchBlockTag(file_tag.public_tag(), 1).index();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
