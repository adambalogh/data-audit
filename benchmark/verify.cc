#include "benchmark/benchmark.h"

#include "audit/client/verify/client.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/providers/local_disk/fetcher.h"
#include "audit/providers/local_disk/file_tag_source.h"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace audit;
using namespace audit::providers;

// TODO build benchmark as Release

static void Verify(benchmark::State& state) {
  while (state.KeepRunning()) {
    verify::Client client{
        std::unique_ptr<verify::FileTagSource>(new local_disk::FileTagSource),
        std::unique_ptr<verify::ProofSource>(new verify::NoServerProofSource{
            std::unique_ptr<server::FetcherFactory>{
                new local_disk::FetcherFactory}})};

    // assert(client.Verify("Effective Modern C++.pdf", 100) == true);
    assert(client.Verify("bioshock.large", 100) == true);
  }
}

BENCHMARK(Verify);

BENCHMARK_MAIN();
