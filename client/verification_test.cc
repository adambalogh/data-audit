#include "gtest/gtest.h"
#include "audit/client/verification.h"

#include <sstream>
#include <vector>

#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/client/file_tag.h"
#include "audit/client/block_tagger.h"
#include "audit/server/prover.h"
#include "audit/util.h"
#include "audit/common.h"
#include "audit/test_util.h"

using namespace audit;

// TODO cleanup
TEST(Verification, Successful) {
  std::stringstream file{"abcdefgh"};
  unsigned int p = 7883;
  std::vector<unsigned int> alphas{342, 53};
  std::vector<unsigned int> blocks{0, 1};
  std::vector<unsigned int> weights{43, 89};

  FileTag file_tag{file, "", 2, 1, make_BN_vector(alphas), BN_new_ptr(p)};
  proto::PublicFileTag public_file_tag = file_tag.PublicProto();
  proto::PrivateFileTag private_file_tag = file_tag.PrivateProto();

  proto::Challenge challenge;
  challenge.set_allocated_file_tag(new proto::PublicFileTag(public_file_tag));
  for (int i = 0; i < blocks.size(); ++i) {
    auto item = challenge.add_items();
    item->set_index(blocks[i]);
    item->set_weight(BignumToString(*BN_new_ptr(weights[i])));
  }

  BlockTagger tagger{file_tag, std::unique_ptr<PRF>{new HMACPRF{"hello"}}};
  std::vector<proto::BlockTag> tags;
  while (tagger.HasNext()) {
    tags.push_back(tagger.GetNext());
  }

  MemoryFetcher fetcher{file_tag, tags, file};
  Prover prover{fetcher, challenge};
  auto proof = prover.Prove();

  Verification v;
  EXPECT_TRUE(v.Verify(private_file_tag, challenge, proof,
                       std::unique_ptr<PRF>{new HMACPRF{"hello"}}));
}

TEST(Verification, FileHasChanged) {
  std::stringstream file{"abcd"};
  unsigned int p = 7883;
  std::vector<unsigned int> alphas{342, 53};
  std::vector<unsigned int> blocks{0, 1};
  std::vector<unsigned int> weights{43, 89};

  FileTag file_tag{file, "", 2, 1, make_BN_vector(alphas), BN_new_ptr(p)};
  proto::PublicFileTag public_file_tag = file_tag.PublicProto();
  proto::PrivateFileTag private_file_tag = file_tag.PrivateProto();

  proto::Challenge challenge;
  challenge.set_allocated_file_tag(new proto::PublicFileTag(public_file_tag));
  for (int i = 0; i < blocks.size(); ++i) {
    auto item = challenge.add_items();
    item->set_index(blocks[i]);
    item->set_weight(BignumToString(*BN_new_ptr(weights[i])));
  }

  BlockTagger tagger{file_tag, std::unique_ptr<PRF>{new HMACPRF{"hello"}}};
  std::vector<proto::BlockTag> tags;
  while (tagger.HasNext()) {
    tags.push_back(tagger.GetNext());
  }

  std::stringstream corrupted_file{"bbcd"};
  MemoryFetcher fetcher{file_tag, tags, corrupted_file};
  Prover prover{fetcher, challenge};
  auto proof = prover.Prove();

  Verification v;
  EXPECT_FALSE(v.Verify(private_file_tag, challenge, proof,
                        std::unique_ptr<PRF>{new HMACPRF{"hello"}}));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
