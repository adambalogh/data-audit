#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/upload/client.h"
#include "audit/client/upload/local_disk_storage.h"
#include "audit/client/prf.h"
#include "audit/client/verify/verify.h"
#include "audit/server/prover.h"
#include "audit/client/verify/local_disk_file_tag_source.h"
#include "audit/server/local_disk_fetcher.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

int main() {
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    return -1;
  }

  Client client{std::unique_ptr<Storage>{new LocalDiskStorage{"hello"}}};

  std::stringstream content{
      "aejfwoigjqogijwer;goit43io;h5w3[94thg39wa;wighe;oiw4h3;"
      "toihtq09rhgwqpg84538h"};

  File file{content, "hello"};
  client.Upload(file);

  proto::Challenge challenge;

  LocalDiskFileTagSource tag_source;
  auto tag = tag_source.GetFileTag("hello");

  *challenge.mutable_file_tag() = tag.public_tag();

  auto item = challenge.add_items();
  item->set_index(0);
  BN_ptr weight{BN_new(), ::BN_free};
  BN_set_word(weight.get(), 1432);
  BignumToString(*weight, item->mutable_weight());

  LocalDiskFetcher fetcher{challenge.file_tag()};
  Prover prover{fetcher, challenge};

  auto proof = prover.Prove();

  if (Verify<HMACPRF>(tag, challenge, proof)) {
    std::cout << "passed!!!" << std::endl;
  } else {
    std::cout << "failed" << std::endl;
  }
}
