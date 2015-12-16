#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/tagger.h"
#include "audit/client/local_disk_storage.h"
#include "audit/client/prf.h"
#include "audit/client/verification.h"
#include "audit/server/prover.h"
#include "audit/server/local_disk_fetcher.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

proto::PrivateFileTag GetFileTag(const std::string& file_name) {
  proto::PrivateFileTag tag;
  std::ifstream tag_file_;
  std::string file_dir_{"/users/adambalogh/Developer/audit/files_dir/"};
  tag_file_.open(file_dir_ + "file_Tag" + file_name);

  std::stringstream buffer;
  buffer << tag_file_.rdbuf();
  tag.ParseFromString(buffer.str());
  return tag;
}

int main() {
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    return -1;
  }

  Tagger tagger{
      std::unique_ptr<Storage>{new LocalDiskStorage{"test"}},
      std::unique_ptr<RandomNumberGenerator>{new CryptoNumberGenerator},
      std::unique_ptr<PRF>{new HMACPRF}};

  std::stringstream file{
      "aejfwoigjqogijwer;goit43io;h5w3[94thg39wa;wighe;oiw4h3;"
      "toihtq09rhgwqpg84538h"};

  tagger.Run(file, "test");

  proto::Challenge challenge;

  auto tag = GetFileTag("test");

  *challenge.mutable_file_tag() = tag.public_tag();

  auto item = challenge.add_items();
  item->set_index(0);
  BN_ptr weight{BN_new(), ::BN_free};
  BN_set_word(weight.get(), 1432);
  BignumToString(*weight, item->mutable_weight());

  LocalDiskFetcher fetcher{challenge.file_tag()};
  Prover prover{fetcher, challenge};

  auto proof = prover.Prove();

  Verification v;
  if (v.Verify(tag, challenge, proof, std::unique_ptr<PRF>{new HMACPRF})) {
    std::cout << "passed!!!" << std::endl;
  } else {
    std::cout << "failed" << std::endl;
  }
}
