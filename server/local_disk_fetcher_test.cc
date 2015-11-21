#include "gtest/gtest.h"
#include "audit/server/local_disk_fetcher.h"

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

TEST(LocalDiskFetcher, FetchBlock) {
  CryptoNumberGenerator gen;
  BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 1111111);

  proto::PublicFileTag tag;
  tag.set_num_sectors(2);
  tag.set_sector_size(2);
  tag.set_file_name("test_text.txt");

  LocalDiskFetcher f{tag};
}

TEST(LocalDiskFetcher, FecthBlockTag) {}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
