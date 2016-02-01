#include "audit/providers/dropbox/storage.h"
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

using json = nlohmann::json;

using web::uri;
using web::http::http_request;
using Concurrency::streams::file_stream;

namespace audit {
namespace dropbox {

const std::string Fetcher::files_dir_{application_dir + "dropboxfiles/"};
const std::string Fetcher::tags_dir_{application_dir + "dropboxtags/"};

Fetcher::Fetcher(TokenSourceInterface& token_source,
                 const proto::PublicFileTag& file_tag)
    : DropboxClient(token_source), server::Fetcher(file_tag) {
  DownloadFile();
  DownloadBlockTagFile();
}

Fetcher::~Fetcher() {
  DeleteFile();
  DeleteBlockTagFile();
}

size_t Fetcher::DownloadAndSaveFile(const std::string& path,
                                    const std::string& out_file_path) {
  json parameters;
  parameters["path"] = path;

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
  DownloadAndSaveFile(Storage::GetFilePath(file_tag_.file_name()),
                      files_dir_ + file_tag_.file_name());
  file_.open(files_dir_ + file_tag_.file_name(), std::ifstream::binary);
  if (!file_) {
    throw std::runtime_error(
        "dropbox::Fetcher could not open file containing downloaded file "
        "content");
  }
}

void Fetcher::DownloadBlockTagFile() {
  DownloadAndSaveFile(Storage::GetBlockTagFilePath(file_tag_.file_name()),
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

std::unique_ptr<std::basic_istream<char>> Fetcher::FetchBlock(
    unsigned long index) {
  size_t block_size = file_tag_.sector_size() * file_tag_.num_sectors();
  file_.seekg(index * block_size);

  std::vector<unsigned char> binary(block_size);
  file_.read((char*)binary.data(), binary.size());

  std::string str{binary.begin(), binary.begin() + file_.gcount()};
  return std::unique_ptr<std::basic_istream<char>>{new std::stringstream{str}};
}

proto::BlockTag Fetcher::FetchBlockTag(unsigned long index) {
  BlockTagMap block_tag_map{file_tag_.block_tag_map()};
  size_t start;
  size_t end;
  std::tie(start, end) = block_tag_map.FindBlockTag(index);

  std::vector<unsigned char> binary(end - start);
  block_tag_file_.seekg(start);
  block_tag_file_.read((char*)binary.data(), binary.size());

  proto::BlockTag tag;
  tag.ParseFromArray(binary.data(), binary.size());
  return tag;
}
}
}
