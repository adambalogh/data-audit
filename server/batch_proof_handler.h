#pragma once

#include "proxygen/httpserver/RequestHandler.h"

#include "audit/server/fetcher.h"
#include "audit/client/verify/no_server_proof_source.h"

namespace audit {
namespace server {

class BatchProofHandler : public proxygen::RequestHandler {
 public:
  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;

 private:
  verify::NoServerProofSource proof_source_{
      std::make_unique<local_disk::FetcherFactory>()};

  std::unique_ptr<folly::IOBuf> body_;
};
}
}
