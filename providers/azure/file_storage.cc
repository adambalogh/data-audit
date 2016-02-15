#include "audit/providers/azure/file_storage.h"

#include <istream>

#include "cpprest/http_client.h"
#include "cpprest/uri.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using web::http::message_direction::direction;
using audit::upload::StorageListener;

namespace audit {
namespace providers {
namespace azure {

void FileStorage::StoreFile(const std::string& file_name, std::istream& stream,
                            StorageListener& listener) {
  UploadFile(file_name, stream, listener);
}

void FileStorage::UploadFile(const std::string& file_name, std::istream& stream,
                             StorageListener& listener) {
  json params;
  params["file_name"] = file_name;

  http_request request{"POST"};
  request.set_request_uri(UPLOAD_URL);
  request.headers().add("Data-Audit-Params", params.dump());

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

  auto response = client_.request(request).get();

  // Read response
  response.extract_string().get();
}
}
}
}
