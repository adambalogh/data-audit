#include "gtest/gtest.h"
#include "audit/providers/dropbox/token_source.h"

#include <string>

#include "audit/common.h"

using namespace audit::providers::dropbox;

void RemoveTokenFile() { std::remove(TokenSource::TOKEN_FILE.c_str()); }
void RemoveSecretsFile() { std::remove(TokenSource::SECRETS_FILE.c_str()); }
void WriteSecretsFile(const std::string& content) {
  std::ofstream file{TokenSource::SECRETS_FILE};
  file << content;
}

class TokenSourceTest : public ::testing::Test {
  void SetUp() override {
    WriteSecretsFile("{\"client_id\":\"1234\", \"client_secret\":\"abcd\"}");
  }
};

TEST_F(TokenSourceTest, NoSecretsFile) {
  RemoveSecretsFile();
  EXPECT_THROW(TokenSource{}, std::runtime_error);
}

TEST_F(TokenSourceTest, InvalidSecretsFile) {
  WriteSecretsFile("a");
  EXPECT_THROW(TokenSource{}, std::runtime_error);
}

TEST_F(TokenSourceTest, NeedToAuthorizeWhenNoTokenFile) {
  RemoveTokenFile();
  TokenSource source;
  EXPECT_TRUE(source.NeedToAuthorize());
}

TEST_F(TokenSourceTest, NeedToAuthorizeWhenTokenFileIsInvalid) {
  {
    std::ofstream token_file{TokenSource::TOKEN_FILE};
    token_file << "a";
  }
  TokenSource source;
  EXPECT_TRUE(source.NeedToAuthorize());
}

TEST_F(TokenSourceTest, RetrieveTokenFromFile) {
  std::string token{"abcdefgh"};
  {
    std::ofstream token_file{TokenSource::TOKEN_FILE};
    token_file << "{\"token\":\"" + token + "\"}";
  }
  TokenSource source;
  EXPECT_FALSE(source.NeedToAuthorize());
  EXPECT_EQ(token, source.GetToken());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
