#include "gtest/gtest.h"
#include "audit/client/verification.h"

#include <sstream>

#include "openssl/bn.h"

#include "audit/proto/cpor.pb.h"
#include "audit/util.h"
#include "audit/common.h"
#include "audit/test_util.h"

using namespace audit;

TEST(Verification, Verify) {
  unsigned int alphas[] = {53};

  // Consturct Public File Tag
  proto::PublicFileTag public_file_tag;
  public_file_tag.set_num_blocks(1);
  public_file_tag.set_num_sectors(1);
  public_file_tag.set_sector_size(1);
  auto p = BN_new_ptr(7883);
  BignumToString(*p, public_file_tag.mutable_p());

  // Construct Alphas
  auto alpha_1 = BN_new_ptr(alphas[0]);

  // Construct Private File Tag
  proto::PrivateFileTag private_file_tag;
  private_file_tag.set_allocated_public_tag(
      new proto::PublicFileTag{public_file_tag});
  *(private_file_tag.add_alphas()) = BignumToString(*alpha_1);

  // Construct Challenge
  proto::Challenge challenge;
  challenge.set_allocated_file_tag(new proto::PublicFileTag(public_file_tag));
  BN_ptr weight{BN_new(), ::BN_free};
  auto item = challenge.add_items();
  item->set_index(0);
  BN_set_word(weight.get(), 13);
  item->set_weight(BignumToString(*weight));

  HMACPRF prf{"code"};

  // Construct Proof
  proto::Proof proof;
  unsigned int file = 100000;
  auto sigma = prf.Encode(0);
  sigma += file * alphas[0];
  sigma %= 7883;
  sigma *= 13;
  sigma %= 7883;
  auto mu = BN_new_ptr(13 * file);
  mu %= 7883;
  BignumToString(*mu, proof.add_mus());
  BignumToString(*sigma, proof.mutable_sigma());

  Verification v;
  EXPECT_TRUE(v.Verify(private_file_tag, challenge, proof,
                       std::unique_ptr<PRF>(new HMACPRF{"code"})));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
