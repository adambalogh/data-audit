#include "audit/providers/dropbox/dropbox_storage.h"

#include <sstream>
#include <iostream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using web::http::client::http_client;

namespace audit {
namespace upload {

const uri DropboxStorage::UPLOAD_URL{"/2/files/upload"};

void DropboxStorage::StoreFile(const std::string& file_name,
                               std::istream& stream,
                               StorageListener& listener) {
  SaveFileToDropbox("/" + file_name, stream, listener);
}

void DropboxStorage::StoreFileTag(const std::string& file_name,
                                  const proto::PrivateFileTag& file_tag,
                                  StorageListener& listener) {
  auto binary = file_tag.SerializeAsString();
  std::stringstream stream{binary};
  SaveFileToDropbox("/FileTags/" + file_name, stream, listener);
}

void DropboxStorage::StoreBlockTagFile(const std::string& file_name,
                                       const std::string& block_file_path,
                                       StorageListener& listener) {
  std::ifstream stream{block_file_path, std::ifstream::binary};
  if (!stream) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }
  SaveFileToDropbox("/Tags/" + file_name, stream, listener);
}

void DropboxStorage::SaveFileToDropbox(const std::string& path,
                                       std::istream& stream,
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

  // TODO Check for error
  auto response = SendRequest(request);
  std::cout << response.to_string() << std::endl;
}
}
}
