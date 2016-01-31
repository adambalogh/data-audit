#include "audit/providers/dropbox/storage.h"

#include <sstream>
#include <iostream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using audit::upload::StorageListener;

namespace audit {
namespace dropbox {

const uri Storage::UPLOAD_URL{"/2/files/upload"};

void Storage::StoreFile(const std::string& file_name, std::istream& stream,
                        StorageListener& listener) {
  SaveFileToDropbox(GetFilePath(file_name), stream, listener);
}

void Storage::StoreFileTag(const std::string& file_name,
                           const proto::PrivateFileTag& file_tag,
                           StorageListener& listener) {
  auto binary = file_tag.SerializeAsString();
  std::stringstream stream{binary};
  SaveFileToDropbox(GetFileTagPath(file_name), stream, listener);
}

void Storage::StoreBlockTagFile(const std::string& file_name,
                                const std::string& block_file_path,
                                StorageListener& listener) {
  std::ifstream stream{block_file_path, std::ifstream::binary};
  if (!stream) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }
  SaveFileToDropbox(GetBlockTagFilePath(file_name), stream, listener);
}

void Storage::SaveFileToDropbox(const std::string& path, std::istream& stream,
                                StorageListener& listener) {
  json parameters;
  parameters["path"] = path;
  parameters["mode"] = "overwrite";
  parameters["autorename"] = false;

  http_request request{"POST"};
  request.set_request_uri(UPLOAD_URL);
  request.headers().add("Dropbox-API-Arg", parameters.dump());
  Concurrency::streams::stdio_istream<uint8_t> c_stream{stream};
  request.set_body(c_stream);

  auto response = SendRequest(request);
  // TODO check returned values
}
}
}
