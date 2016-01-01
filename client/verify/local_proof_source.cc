#include "audit/client/verify/local_proof_source.h"

#include "audit/server/local_disk_fetcher.h"
#include "audit/server/prover.h"

namespace audit {
namespace verify {

proto::Proof LocalProofSource::GetProof(const proto::Challenge& challenge) {
  LocalDiskFetcher fetcher{challenge.file_tag()};
  Prover prover{fetcher, challenge};
  return prover.Prove();
}
}
}
