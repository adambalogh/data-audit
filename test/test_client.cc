#include "test_client.h"

#include "audit/common.h"
#include "audit/test_util.h"
#include "audit/util.h"
#include "audit/proto/cpor.pb.h"
#include "audit/client/prf.h"
#include "audit/client/file_tag.h"
#include "audit/client/block_tagger.h"
#include "audit/server/local_disk_fetcher.h"
#include "audit/server/prover.h"
#include "audit/client/verification.h"

using namespace audit;

TestClient::TestClient(const std::string& content) {
  std::ofstream test_file{full_file_name_};
  files_to_delete.push_back(full_file_name_);
  test_file << content;
}

TestClient::~TestClient() {
  for (auto& file : files_to_delete) {
    std::remove(file.c_str());
  }
}

void TestClient::Run() {
  std::ifstream test_file{full_file_name_};

  CryptoNumberGenerator gen;
  BN_ptr p{BN_new(), ::BN_free};
  BN_set_word(p.get(), 295075147);
  FileTag tag{test_file, "test_file", 3, 2, std::move(p), &gen};
  std::unique_ptr<PRF> prf{new HMACPRF{"hello"}};
  BlockTagger tagger{tag, std::move(prf)};

  while (tagger.HasNext()) {
    auto tag = tagger.GetNext();

    std::string bytes;
    tag.SerializeToString(&bytes);
    std::ofstream tag_file{file_path_ + "tags" + file_name_ +
                           std::to_string(tag.index())};
    tag_file << bytes;
    files_to_delete.push_back(file_path_ + "tags" + file_name_ +
                              std::to_string(tag.index()));
  }
  file_tag = tag.PublicProto();
  private_file_tag = tag.PrivateProto();

  proto::Challenge challenge;
  *(challenge.mutable_file_tag()) = file_tag;

  for (int i = 0; i < file_tag.num_blocks(); ++i) {
    auto item = challenge.add_items();
    item->set_index(i);

    BN_ptr weight{BN_new(), ::BN_free};
    BN_set_word(weight.get(), 1);

    BignumToString(*weight, item->mutable_weight());
  }

  LocalDiskFetcher f{file_tag, file_path_};
  Prover prover{f, challenge};

  auto proof = prover.Prove();

  Verification v;
  std::unique_ptr<PRF> prf2{new HMACPRF{"hello"}};
  std::cout << v.Verify(private_file_tag, challenge, proof, std::move(prf2))
            << std::endl;
}
