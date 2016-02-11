#include "audit/server/proof_handler.h"

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

void ProofHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {}

void ProofHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    if (body_) {
        body_->prependChain(std::move(body));
    } else {
        body_ = std::move(body);
    }
}

void ProofHandler::onEOM() noexcept {
    proto::Challenge challenge;
    challenge.ParseFromArray(body_->data(), body_->length());

    fetcher_.reset(new local_disk::Fetcher(challenge.file_tag()));
    Prover prover{*fetcher_, challenge};
    auto proof = prover.Prove();

    auto response_body = IOBuf::create(proof.ByteSize());
    proof.SerializeToArray(response_body->writableData(),
                           response_body->capacity());

    ResponseBuilder(downstream_)
        .status(200, "OK")
        .body(std::move(response_body))
        .sendWithEOM();
}

void ProofHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void ProofHandler::requestComplete() noexcept { delete this; }

void ProofHandler::onError(ProxygenError err) noexcept { delete this; }
}
}
