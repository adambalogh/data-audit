#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"

#include <string>

#include "audit/common.h"

using namespace audit::providers::dropbox;

void RemoveTokenFile() { std::remove(TokenSource::TOKEN_FILE.c_str()); }

TEST(TokenSource, NeedToAuthorizeWhenNoTokenFile) {
  RemoveTokenFile();
  TokenSource source;
  EXPECT_TRUE(source.NeedToAuthorize());
}

TEST(TokenSource, NeedToAuthorizeWhenTokenFileIsInvalid) {
  {
    std::ofstream token_file{TokenSource::TOKEN_FILE};
    token_file << "ferigrjogierje";
  }
  TokenSource source;
  EXPECT_TRUE(source.NeedToAuthorize());
}

TEST(TokenSource, RetrieveTokenFromFile) {
  std::string token{"abcdefgh"};
  {
    std::ofstream token_file{TokenSource::TOKEN_FILE};
    token_file << "{\"token\":\"" + token + "\"}";
  }
  TokenSource source;
  EXPECT_FALSE(source.NeedToAuthorize());
  EXPECT_EQ(token, source.GetToken());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
