#include "audit/providers/dropbox/file_storage.h"

#include <sstream>
#include <iostream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

#include "audit/providers/dropbox/dropbox_urls.h"

using namespace web::http::message_direction;

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using audit::upload::StorageListener;

namespace audit {
namespace providers {
namespace dropbox {

void FileStorage::SaveFileToDropbox(const std::string& path,
                                    std::istream& stream,
                                    StorageListener& listener) {
  json parameters;
  parameters["path"] = "/" + path;
  parameters["mode"] = "overwrite";
  parameters["autorename"] = false;

  http_request request{"POST"};
  request.set_request_uri(UPLOAD_PATH);
  request.headers().add("Dropbox-API-Arg", parameters.dump());
  Concurrency::streams::stdio_istream<uint8_t> c_stream{stream};
  request.set_body(c_stream);
  size_t bytes_uploaded = 0;
  request.set_progress_handler(
      [&listener, &bytes_uploaded](direction d, size_t bytes) {
        if (d == direction::upload) {
          listener.OnChunkStored(bytes - bytes_uploaded);
          bytes_uploaded = bytes;
        }
      });

  auto response = SendRequest(request);
  // TODO check returned values
}
}
}
}
