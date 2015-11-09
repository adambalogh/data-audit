#include "gtest/gtest.h"
#include "audit/server/prover.h"

#include <iostream>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/block_tagger.h"
#include "audit/proto/cpor.pb.h"

#include <sstream>

using namespace audit;

class DummyFetcher : public Fetcher {
 public:
  DummyFetcher(const FileTag& file_tag, std::vector<proto::BlockTag>& tags,
               std::stringstream& s)
      : file_tag_(file_tag), tags_(tags), s_(s) {}
  std::basic_istream<char, std::char_traits<char>>& FetchBlock(
      unsigned long index) {
    std::string block{
        s_.str().data() + BlockSize() * index,
        std::min(BlockSize(), s_.str().size() - (BlockSize() * index))};
    stream.str(block);
    return stream;
  }

  proto::BlockTag FetchBlockTag(unsigned long index) { return tags_.at(index); }

 private:
  unsigned long BlockSize() {
    return file_tag_.num_sectors * file_tag_.sector_size;
  }

  const FileTag& file_tag_;
  std::istringstream stream;
  std::vector<proto::BlockTag>& tags_;
  std::stringstream& s_;
};

TEST(Prover, Prove) {
  std::stringstream file{
      "jdoigeowjoijJOJIOJRjegoirjgiergjeogjow f;erigje;gi j;gioewj "
      ";eriojwasgpewpoeir34op3i"};

  CryptoNumberGenerator g;
  BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 353868019);
  FileTag file_tag{3, 3, std::move(p), &g};
  std::unique_ptr<PRF> prf{new SiphashPRF{"hello"}};

  BlockTagger block_tagger{file, &file_tag, std::move(prf)};

  std::vector<proto::BlockTag> tags;
  while (block_tagger.HasNext()) {
    tags.push_back(block_tagger.GetNext());
  }

  proto::Challenge challenge;
  auto item = challenge.add_items();
  item->set_index(0);
  item->set_weight("abcd");

  item = challenge.add_items();
  item->set_index(2);
  item->set_weight("abcdw");

  DummyFetcher fetcher{file_tag, tags, file};

  Prover prover;
  auto proof = prover.Prove(file_tag.Proto(), fetcher, challenge);

  std::cout << proof.DebugString() << std::endl;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
