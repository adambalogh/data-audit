#include "audit/server/storage_handler.h"

#include <string>

#include "folly/io/IOBuf.h"
#include "proxygen/httpserver/ResponseBuilder.h"
#include "nlohmann/json.hpp"

#include "audit/common.h"

using json = nlohmann::json;

using namespace proxygen;

using folly::IOBuf;

namespace audit {
namespace server {

void StorageHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {
  auto params_str = headers->getHeaders().getSingleOrEmpty("Data-Audit-Params");
  if (params_str == empty_string) {
    // error
  }
  auto params = json::parse(params_str);
  auto it = params.find("file_name");
  if (it == params.end()) {
    // error
  }
  auto file_name = it->get<std::string>();
  out_file_.open(application_dir + file_name, std::ofstream::binary);
  if (!out_file) {
    std::cout << "could not open file" << std::endl;
  }
}

void StorageHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  out_file_.write(reinterpret_cast<const char*>(body->data()), body->length());
}

void StorageHandler::onEOM() noexcept {
  ResponseBuilder(downstream_).status(200, "OK").sendWithEOM();
}

void StorageHandler::onUpgrade(UpgradeProtocol protocol) noexcept {}

void StorageHandler::requestComplete() noexcept { delete this; }

void StorageHandler::onError(ProxygenError err) noexcept { delete this; }
}
}
