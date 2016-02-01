#pragma once

#include "audit/server/fetcher.h"
#include "audit/block_tag_map.h"
#include "audit/providers/dropbox/dropbox_client.h"

namespace audit {
namespace dropbox {

class Fetcher : public DropboxClient, public server::Fetcher {
 public:
  Fetcher(TokenSourceInterface& token_source,
          const proto::PublicFileTag& file_tag);

  ~Fetcher();

  std::unique_ptr<std::basic_istream<char>> FetchBlock(
      unsigned long index) override;

  proto::BlockTag FetchBlockTag(unsigned long index) override;

  void DownloadAndSaveFile(const std::string& path,
                           const std::string& out_file_path);
  void DownloadFile();
  void DownloadBlockTagFile();

  void DeleteFile();
  void DeleteBlockTagFile();

 private:
  const static std::string files_dir_;
  const static std::string tags_dir_;

  std::ifstream file_;
  std::ifstream block_tag_file_;
};

class FetcherBuilder : public server::FetcherBuilder {
 public:
  FetcherBuilder(TokenSource& token_source) : token_source_(token_source) {}

  std::unique_ptr<server::Fetcher> build(
      const proto::PublicFileTag& file_tag) override {
    return std::unique_ptr<server::Fetcher>{
        new Fetcher{token_source_, file_tag}};
  }

 private:
  TokenSource& token_source_;
};
}
}
