#pragma once

#include "proxygen/httpserver/RequestHandler.h"

#include "audit/server/fetcher.h"
#include "audit/providers/local_disk/file_list_source.h"

namespace audit {
namespace server {

class FileListHandler : public proxygen::RequestHandler {
   public:
    void onRequest(
        std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

   private:
    audit::providers::local_disk::FileListSource source_;
};
}
}
