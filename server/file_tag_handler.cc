#include "audit/server/file_tag_handler.h"

#include <string>
#include <iostream>

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/ResponseBuilder.h"
#include "nlohmann/json.hpp"

#include "audit/common.h"
#include "audit/providers/local_disk/file_list_source.h"

using json = nlohmann::json;

using namespace proxygen;

using folly::IOBuf;

namespace audit {
namespace server {

void FileTagHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {}

void FileTagHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    body_.append(reinterpret_cast<const char*>(body->data()), body->length());
}

void FileTagHandler::onEOM() noexcept {
    auto params = json::parse(body_);
    auto file_name = params.at("file_name").get<std::string>();
    const auto file_tag = source_.GetFileTag(file_name);
    size_t file_tag_size = file_tag.ByteSize();

    auto bin = file_tag.SerializeAsString();
    auto response_body = IOBuf::copyBuffer(bin);

    ResponseBuilder(downstream_)
        .status(200, "OK")
        .body(std::move(response_body))
        .sendWithEOM();
}

void FileTagHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void FileTagHandler::requestComplete() noexcept { delete this; }

void FileTagHandler::onError(ProxygenError err) noexcept { delete this; }
}
}
