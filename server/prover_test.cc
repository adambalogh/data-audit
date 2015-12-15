#include "gtest/gtest.h"
#include "audit/server/prover.h"

#include <iostream>
#include <vector>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/file_tag.h"
#include "audit/client/block_tagger.h"
#include "audit/proto/cpor.pb.h"
#include "audit/test_util.h"

#include <sstream>

using namespace audit;

TEST(Prover, Sigma) {
  std::stringstream file{"abcdefghijklmno\npqr "};

  BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 353868019);

  CryptoNumberGenerator g;
  std::vector<BN_ptr> alphas;
  for (int i = 0; i < 2; ++i) {
    alphas.push_back(g.GenerateNumber(*p));
  }

  FileTag file_tag{file, "", 2, 2, std::move(alphas), std::move(p)};
  std::unique_ptr<PRF> prf{new HMACPRF{"hello"}};

  BlockTagger block_tagger{file_tag, std::move(prf)};

  std::vector<proto::BlockTag> tags;
  while (block_tagger.HasNext()) {
    tags.push_back(block_tagger.GetNext());
  }

  proto::Challenge challenge;
  *challenge.mutable_file_tag() = file_tag.PublicProto();

  auto item = challenge.add_items();
  item->set_index(0);
  BN_ptr weight1{BN_new(), ::BN_free};
  BN_set_word(weight1.get(), 101);
  item->set_weight(BignumToString(*weight1));

  item = challenge.add_items();
  item->set_index(2);
  BN_ptr weight2{BN_new(), ::BN_free};
  BN_set_word(weight2.get(), 17);
  item->set_weight(BignumToString(*weight2));

  MemoryFetcher fetcher{file_tag, tags, file};
  Prover prover{fetcher, challenge};
  auto proof = prover.Prove();

  BN_ptr sigma_sum{BN_new(), ::BN_free};
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};

  BN_mul(weight1.get(), weight1.get(), StringToBignum(tags[0].sigma()).get(),
         ctx.get());
  BN_mul(weight2.get(), weight2.get(), StringToBignum(tags[2].sigma()).get(),
         ctx.get());
  BN_add(sigma_sum.get(), weight1.get(), weight2.get());
  BN_mod(sigma_sum.get(), sigma_sum.get(), file_tag.p(), ctx.get());

  ASSERT_EQ(0, BN_cmp(sigma_sum.get(), StringToBignum(proof.sigma()).get()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
