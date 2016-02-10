#pragma once

#include "proxygen/httpserver/RequestHandler.h"

#include "audit/server/fetcher.h"

namespace audit {
namespace server {

class ProofHandler : public proxygen::RequestHandler {
 public:
  ProofHandler(Fetcher& fetcher) : fetcher_(fetcher) {}

  void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) override;

  void onBody(std::unique_ptr<folly::IOBuf> body) override;

  void onEOM() override;

  void onUpgrade(proxygen::UpgradeProtocol proto) override;

  void requestComplete() override;

  void onError(proxygen::ProxygenError err) override;

 private:
  Fetcher& fetcher_;

  std::unique_ptr<folly::IOBuf> body_;
};
}
}
