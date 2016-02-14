#include "audit/providers/azure/storage.h"

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

void Storage::StoreFile(const std::string& file_name, std::istream& stream,
                        StorageListener& listener) {
  UploadFile(GetFilePath(file_name), stream, listener);
}

void Storage::StoreFileTag(const std::string& file_name,
                           const proto::PrivateFileTag& file_tag,
                           StorageListener& listener) {
  auto binary = file_tag.SerializeAsString();
  std::stringstream stream{binary};
  UploadFile(GetFileTagPath(file_name), stream, listener);
}

void Storage::StoreBlockTagFile(const std::string& file_name,
                                const std::string& block_file_path,
                                StorageListener& listener) {
  std::ifstream stream{block_file_path, std::ifstream::binary};
  if (!stream) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }
  UploadFile(GetBlockTagFilePath(file_name), stream, listener);
}

void Storage::UploadFile(const std::string& file_name, std::istream& stream,
                         StorageListener& listener) {
  json params;
  params["file_name"] = file_name;

  http_request request{"POST"};
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
}
}
}
}
