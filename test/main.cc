#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/providers/local_disk/local_disk_storage.h"
#include "audit/providers/local_disk/local_disk_file_tag_source.h"
#include "audit/providers/dropbox/dropbox_storage.h"
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

  dropbox::TokenSource token_source{[]() {
    std::string code;
    std::cin >> code;
    return code;
  }};

  upload::Client upload_client{std::unique_ptr<upload::ReusableStorage>{
      new upload::DropboxStorage{token_source}}};

  std::stringstream content{
      "aejfwoigjqogijwer;goit43io;h5w3[94thg39wa;wighe;oiw4h3;"
      "toihtq09rhgwqpg84538hewwf43t34ti43to5tij5ijot4ijf"};

  upload::File file{content, file_name};
  upload_client.Upload(
      file, [](int percentage) { std::cout << percentage << std::endl; });

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
