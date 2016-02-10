#pragma once

#include "proxygen/httpserver/RequestHandler.h"

#include "audit/server/fetcher.h"

namespace audit {
namespace server {

class ProofHandler : public proxygen::RequestHandler {
   public:
    ProofHandler(Fetcher& fetcher) : fetcher_(fetcher) {}

    void onRequest(
        std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

   private:
    Fetcher& fetcher_;

    std::unique_ptr<folly::IOBuf> body_;
};
}
}
