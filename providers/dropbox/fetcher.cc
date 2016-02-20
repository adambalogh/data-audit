#include "audit/providers/dropbox/fetcher.h"

#include <iostream>
#include <sstream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "cpprest/filestream.h"
#include "nlohmann/json.hpp"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"
#include "audit/providers/dropbox/file_storage.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using Concurrency::streams::file_stream;

namespace audit {
namespace providers {
namespace dropbox {

const std::string Fetcher::files_dir_{application_dir + "dropboxfiles/"};
const std::string Fetcher::tags_dir_{application_dir + "dropboxtags/"};

Fetcher::Fetcher(TokenSourceInterface& token_source,
                 const proto::PublicFileTag& file_tag)
    : DropboxClient(token_source),
      audit::providers::local_disk::Fetcher(file_tag) {}

Fetcher::~Fetcher() {
  DeleteFile();
  DeleteBlockTagFile();
}

void Fetcher::Setup() {
  DownloadFile();
  DownloadBlockTagFile();
}

size_t Fetcher::DownloadAndSaveFile(const std::string& path,
                                    const std::string& out_file_path) {
  json parameters;
  parameters["path"] = "/" + path;

  http_request request{"GET"};
  request.set_request_uri(DOWNLOAD_PATH);
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
  auto body = response.body();

  auto out_file = file_stream<unsigned char>::open_ostream(
                      out_file_path, std::ios_base::binary).get();

  auto size = body.read_to_end(out_file.streambuf());
  // Wait until file is downloaded
  return size.get();
}

void Fetcher::DownloadFile() {
  DownloadAndSaveFile(upload::Storage::GetFilePath(file_tag_.file_name()),
                      files_dir_ + file_tag_.file_name());
  file_.open(files_dir_ + file_tag_.file_name(), std::ifstream::binary);
  if (!file_) {
    throw std::runtime_error(
        "dropbox::Fetcher could not open file containing downloaded file "
        "content");
  }
}

void Fetcher::DownloadBlockTagFile() {
  DownloadAndSaveFile(
      upload::Storage::GetBlockTagFilePath(file_tag_.file_name()),
      tags_dir_ + file_tag_.file_name());
  block_tag_file_.open(tags_dir_ + file_tag_.file_name(),
                       std::ifstream::binary);
  if (!block_tag_file_) {
    throw std::runtime_error(
        "dropbox::Fetcher could not open file containing donwloaded BlockTag "
        "file");
  }
}

void Fetcher::DeleteFile() {
  std::remove((files_dir_ + file_tag_.file_name()).c_str());
}

void Fetcher::DeleteBlockTagFile() {
  std::remove((tags_dir_ + file_tag_.file_name()).c_str());
}
}
}
}
