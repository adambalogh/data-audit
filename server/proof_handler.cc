#include "audit/server/proof_handler.h"

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/ResponseBuilder.h"

#include "audit/proto/cpor.pb.h"

namespace audit {
namespace server {

void ProofHandler::onRequest(std::unique_ptr<HTTPMessage> headers) {
  stats_->recordRequest();
}

void ProofHandler::onBody(std::unique_ptr<folly::IOBuf> body) {
  if (body_) {
    body_->prependChain(std::move(body));
  } else {
    body_ = std::move(body);
  }
}

void ProofHandler::onEOM() {
  proto::Challenge challenge;
  challenge.ParseFromArray(body_.data(), body_.tail());

  Prover prover{fetcher_, challenge};
  auto proof = prover.Prove();

  auto response_body = IOBuf::create(proof.ByteSize());
  proof.SerializeToArray(response_body.data(), response_body.capacity());

  ResponseBuilder(downstream_)
      .status(200, "OK")
      .body(std::move(response_body))
      .sendWithEOM();
}

void ProofHandler::onUpgrade(UpgradeProtocol protocol) {}

void ProofHandler::requestComplete() { delete this; }

void ProofHandler::onError(ProxygenError err) { delete this; }
}
}
