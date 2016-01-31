#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/fetcher.h"

#include <iostream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

#include "audit/common.h"
#include "audit/proto/cpor.pb.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace dropbox {

const std::string Fetcher::files_dir_{application_dir + "dropboxfiles/"};
const std::string Fetcher::tags_dir_{application_dir + "dropboxtags/"};

Fetcher::~Fetcher() {}

void Fetcher::DownloadFile() {
  json parameters;
  parameters["path"] = Storage::GetFilePath(file_tag_.file_name());

  http_request request{"GET"};
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
}

void Fetcher::DownloadBlockTagFile() {
  json parameters;
  parameters["path"] = Storage::GetBlockTagFilePath(file_tag_.file_name());

  http_request request{"GET"};
  request.headers().add("Dropbox-API-Arg", parameters.dump());

  auto response = SendRequest(request);
}

// TODO this is extremely slow
std::unique_ptr<std::basic_istream<char>> Fetcher::FetchBlock(
    unsigned long index) {}

proto::BlockTag Fetcher::FetchBlockTag(unsigned long index) {
  BlockTagMap block_tag_map{file_tag_.block_tag_map()};
  size_t start;
  size_t end;
  std::tie(start, end) = block_tag_map.FindBlockTag(index);

  proto::BlockTag tag;
  tag.ParseFromArray(binary.data(), binary.size());
  return tag;
}
}
}
