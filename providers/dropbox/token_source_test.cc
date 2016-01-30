#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"

using namespace audit::dropbox;

// IMPORTANT: This test should not be run with Bazel because it doesn't let us
// enter the received code from Dropbox.

TEST(TokenSource, GetToken) {
  TokenSource source{[]() {
    std::string code;
    std::cin >> code;
    return code;
  }};
  std::cout << source.GetToken();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
