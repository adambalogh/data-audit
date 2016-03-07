#include "audit/server/batch_proof_handler.h"

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/ResponseBuilder.h"

#include "audit/proto/cpor.pb.h"
#include "audit/server/prover.h"
#include "audit/providers/local_disk/fetcher.h"

using namespace proxygen;
using namespace audit::providers;

using folly::IOBuf;

namespace audit {
namespace server {

void BatchProofHandler::onRequest(
    std::unique_ptr<HTTPMessage> headers) noexcept {}

void BatchProofHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  if (body_) {
    body_->prependChain(std::move(body));
  } else {
    body_ = std::move(body);
  }
}

void BatchProofHandler::onEOM() noexcept {
  body_->coalesce();

  proto::BatchChallenge batch_challenge;
  batch_challenge.ParseFromArray(body_->data(), body_->length());

  auto proofs = proof_source_.BatchGetProof(batch_challenge);

  auto bin = proofs.SerializeAsString();
  auto response_body = IOBuf::copyBuffer(bin);

  ResponseBuilder(downstream_)
      .status(200, "OK")
      .body(std::move(response_body))
      .sendWithEOM();
}

void BatchProofHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void BatchProofHandler::requestComplete() noexcept { delete this; }

void BatchProofHandler::onError(ProxygenError err) noexcept { delete this; }
}
}
