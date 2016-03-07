#include "audit/providers/azure/proof_source.h"

#include "cpprest/uri.h"
#include "cpprest/http_client.h"

#include "audit/providers/azure/urls.h"

using web::http::http_request;

namespace audit {
namespace providers {
namespace azure {

proto::Proof ProofSource::GetProof(const proto::Challenge& challenge) {
  auto challenge_str = challenge.SerializeAsString();

  // TODO authentication
  http_request request{"GET"};
  request.set_request_uri(PROVE_URL);
  request.set_body(challenge_str);

  auto response = client_.request(request).get();
  auto body_str = response.extract_string(true).get();

  proto::Proof proof;
  if (!proof.ParseFromString(body_str)) {
    // error
  }
  return proof;
}

proto::BatchProof ProofSource::BatchGetProof(
    const proto::BatchChallenge& challenge) {
  // TODO authentication
  http_request request{"GET"};
  request.set_request_uri(BATCH_PROVE_URL);
  request.set_body(challenge.SerializeAsString());

  auto response = client_.request(request).get();
  auto body_str = response.extract_string(true).get();

  proto::BatchProof proof;
  if (!proof.ParseFromString(body_str)) {
    // error
  }
  return std::move(proof);
}
}
}
}
