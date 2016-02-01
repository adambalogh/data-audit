#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/providers/local_disk/local_disk_storage.h"
#include "audit/providers/local_disk/local_disk_file_tag_source.h"
#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_tag_source.h"
#include "audit/server/fetcher.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/proof_source.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

int main() {
  std::string file_name = "test";
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    return -1;
  }

  dropbox::TokenSourceInstance::Get().Initialize([]() {
    std::string code;
    std::cin >> code;
    return code;
  });

  upload::Client upload_client{std::unique_ptr<upload::ReusableStorage>{
      new dropbox::Storage{dropbox::TokenSourceInstance::Get()}}};

  std::stringstream content{"thisisiatestfile"};

  upload::File file{content, file_name};
  upload_client.Upload(file, [](int percentage) {});

  verify::Client verify_client{
      std::unique_ptr<verify::FileTagSource>(
          new dropbox::FileTagSource{dropbox::TokenSourceInstance::Get()}),
      std::unique_ptr<verify::ProofSource>(new verify::NoServerProofSource{
          std::unique_ptr<server::FetcherFactory>{new dropbox::FetcherFactory{
              dropbox::TokenSourceInstance::Get()}}})};

  if (verify_client.Verify(file_name, 100)) {
    std::cout << "passed!!!" << std::endl;
  } else {
    std::cout << "failed" << std::endl;
  }
}
