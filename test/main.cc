#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/client/upload/local_disk_storage.h"
#include "audit/client/verify/local_disk_file_tag_source.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/proof_source.h"
#include "audit/client/verify/local_proof_source.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

int main() {
  std::string file_name = "hello";
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    return -1;
  }

  upload::Client upload_client{
      std::unique_ptr<upload::Storage>{new upload::LocalDiskStorage}};

  std::stringstream content{
      "aejfwoigjqogijwer;goit43io;h5w3[94thg39wa;wighe;oiw4h3;"
      "toihtq09rhgwqpg84538h"};

  upload::File file{content, file_name};
  upload_client.Upload(file);

  verify::Client verify_client{
      std::unique_ptr<verify::FileTagSource>(
          new verify::LocalDiskFileTagSource{}),
      std::unique_ptr<verify::ProofSource>(new verify::LocalProofSource)};

  if (verify_client.Verify(file_name, 100)) {
    std::cout << "passed!!!" << std::endl;
  } else {
    std::cout << "failed" << std::endl;
  }
}
