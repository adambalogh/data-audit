#include <iostream>
#include <sstream>

#include "openssl/rand.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/server/fetcher.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/proof_source.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/proto/cpor.pb.h"
#include "audit/providers/local_disk/file_storage.h"
#include "audit/providers/local_disk/file_tag_source.h"
#include "audit/providers/local_disk/fetcher.h"
#include "audit/client/verify/stats.h"

using namespace audit;
using namespace audit::providers;

int main() {
  std::string file_name = "test";
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    return -1;
  }

  upload::Client upload_client{
      std::unique_ptr<upload::FileStorage>{new local_disk::FileStorage}};

  std::stringstream content{"thisisiatestfile"};

  upload::File file{content, file_name};
  upload_client.Upload(
      file, [](int percentage) { std::cout << percentage << std::endl; });

  verify::Client verify_client{
      std::make_unique<local_disk::FileTagSource>(),
      std::make_unique<verify::NoServerProofSource>(
          std::make_unique<local_disk::FetcherFactory>())};

  verify::Stats stats;
  if (verify_client.Verify(file_name, 100, [](std::string) {}, stats)) {
    std::cout << "passed!!!" << std::endl;
  } else {
    std::cout << "failed" << std::endl;
  }
}
