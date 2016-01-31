#pragma once

#include "audit/server/fetcher.h"
#include "audit/block_tag_map.h"
#include "audit/providers/dropbox/dropbox_client.h"

namespace audit {
namespace dropbox {

class Fetcher : public DropboxClient, public audit::Fetcher {
 public:
  Fetcher(TokenSourceInterface& token_source) : DropboxClient(token_source) {}

  std::unique_ptr<std::basic_istream<char>> FetchBlock(
      const proto::PublicFileTag& file_tag, unsigned long index) override;

  proto::BlockTag FetchBlockTag(const proto::PublicFileTag& file_tag,
                                unsigned long index) override;
};
}
}
