#include "benchmark/benchmark.h"

#include "audit/client/verify/client.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/providers/local_disk/fetcher.h"
#include "audit/providers/local_disk/storage.h"
#include "audit/providers/local_disk/file_tag_source.h"
#include "audit/client/upload/client.h"

#include <assert.h>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

using namespace audit;
using namespace audit::providers;

// TODO build benchmark as Release
static std::vector<std::string> files;

static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

static char RandomChar() { return alphanum[rand() % (sizeof(alphanum) - 1)]; }

static void SetUpFiles() {
  upload::Client upload_client{
      std::unique_ptr<upload::ReusableStorage>{new local_disk::Storage}};

  // 1MB, 10MB, 100MB
  for (int i = 0; i < 3; ++i) {
    size_t megabytes = pow(10, i);
    files.push_back(std::to_string(megabytes) + " MBs");

    size_t bytes = megabytes * 1000 * 1000;
    std::string str(bytes, 'a');
    std::generate(str.begin(), str.end(), RandomChar);
    std::stringstream content{str};

    upload::File file{content, files[i]};
    upload_client.Upload(file, [](int) {});
  }
}

static void DeleteFiles() {
  for (auto file : files) {
    std::remove(local_disk::Storage::GetFilePath(file).c_str());
    std::remove(local_disk::Storage::GetBlockTagFilePath(file).c_str());
    std::remove(local_disk::Storage::GetFileTagPath(file).c_str());
  }
}

static void Verify(benchmark::State& state) {
  verify::Client client{
      std::unique_ptr<verify::FileTagSource>(new local_disk::FileTagSource),
      std::unique_ptr<verify::ProofSource>(new verify::NoServerProofSource{
          std::unique_ptr<server::FetcherFactory>{
              new local_disk::FetcherFactory}})};

  while (state.KeepRunning()) {
    assert(client.Verify(files[state.range_x()], 100) == true);
  }
}

BENCHMARK(Verify)->Arg(0)->Arg(1)->Arg(2);

int main(int argc, char** argv) {
  srand(time(NULL));
  ::benchmark::Initialize(&argc, argv);
  SetUpFiles();
  ::benchmark::RunSpecifiedBenchmarks();
  DeleteFiles();
}
