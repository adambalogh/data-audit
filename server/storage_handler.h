#pragma once

#include <fstream>

#include "proxygen/httpserver/RequestHandler.h"

#include "audit/providers/local_disk/storage.h"

namespace audit {
namespace server {

class StorageHandler : public proxygen::RequestHandler {
   public:
    void onRequest(
        std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

   private:
    std::unique_ptr<folly::IOBuf> body_;

    std::ofstream out_file_;
};
}
}
