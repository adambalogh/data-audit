#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/fetcher.h"

#include <iostream>

#include "cpprest/uri.h"
#include "cpprest/http_client.h"
#include "cpprest/interopstream.h"
#include "nlohmann/json.hpp"

#include "audit/proto/cpor.pb.h"

using json = nlohmann::json;

using web::uri;
using web::http::http_request;

namespace audit {
namespace dropbox {

// TODO this is extremely slow
std::unique_ptr<std::basic_istream<char>> Fetcher::FetchBlock(
    const proto::PublicFileTag& file_tag, unsigned long index) {
  std::cout << "requesting " << index << std::endl;
  http_request request{"GET"};

  auto block_size = file_tag.num_sectors() * file_tag.sector_size();

  request.headers().add(
      "Range", "bytes=" + std::to_string(index * block_size) + "-" +
                   std::to_string(std::min(static_cast<unsigned long long>(
                                               (index + 1) * block_size),
                                           file_tag.file_size() - 1)));
  request.set_request_uri(uri{V1_DOWNLOAD_PATH.to_string() +
                              Storage::GetFilePath(file_tag.file_name())});

  auto response = SendRequest(request);
  auto binary = response.extract_vector().get();
  std::string s_binary{(char*)binary.data(), binary.size()};

  return std::unique_ptr<std::basic_istream<char>>{
      new std::stringstream{std::move(s_binary)}};
}

proto::BlockTag Fetcher::FetchBlockTag(const proto::PublicFileTag& file_tag,
                                       unsigned long index) {
  BlockTagMap block_tag_map{file_tag.block_tag_map()};
  size_t start;
  size_t end;
  std::tie(start, end) = block_tag_map.FindBlockTag(index);

  http_request request{"GET"};

  // Only download the range of the file that contains the requested block
  // TODO find out if it's better to download the whole file
  request.headers().add("Range", "bytes=" + std::to_string(start) + "-" +
                                     std::to_string(end - 1));

  request.set_request_uri(
      uri{V1_DOWNLOAD_PATH.to_string() +
          Storage::GetBlockTagFilePath(file_tag.file_name())});

  auto response = SendRequest(request);
  auto binary = response.extract_vector().get();

  proto::BlockTag tag;
  tag.ParseFromArray(binary.data(), binary.size());
  return tag;
}
}
}
