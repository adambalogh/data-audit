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

 private:
  // Downloads and saves the requeted file to the given path
  void DownloadAndSaveFile(const std::string& path,
                           const std::string& out_file_path);

  // Download local copies from Dropbox
  void DownloadFile();
  void DownloadBlockTagFile();

  // Delete local copies
  void DeleteFile();
  void DeleteBlockTagFile();

  // The directory where the local copies of files are saved to
  const static std::string files_dir_;

  // the directory where the local copies of BlockTag files are saved to
  const static std::string tags_dir_;

  // The file containing the local copy of the requested file
  std::ifstream file_;

  // The file containing the local copy of the BlockTag file of the requested
  // file
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
