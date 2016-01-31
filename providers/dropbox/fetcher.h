#pragma once

#include "audit/server/fetcher.h"
#include "audit/block_tag_map.h"
#include "audit/providers/dropbox/dropbox_client.h"

namespace audit {
namespace dropbox {

class Fetcher : public DropboxClient, public server::Fetcher {
 public:
  Fetcher(TokenSourceInterface& token_source,
          const proto::PublicFileTag file_tag)
      : DropboxClient(token_source), server::Fetcher(file_tag) {}

  ~Fetcher() {}

  std::unique_ptr<std::basic_istream<char>> FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

  void DownloadFile();
  void DownloadBlockTagFile();

 private:
  const static std::string files_dir_;
  const static std::string tags_dir_;
  ;
};
}
}
