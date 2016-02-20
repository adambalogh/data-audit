#pragma once

#include "audit/server/fetcher.h"
#include "audit/server/block_tag_map.h"
#include "audit/providers/dropbox/dropbox_client.h"
#include "audit/providers/local_disk/fetcher.h"

namespace audit {
namespace providers {
namespace dropbox {

class Fetcher : public DropboxClient,
                public ::audit::providers::local_disk::Fetcher {
 public:
  Fetcher(TokenSourceInterface& token_source,
          const proto::PublicFileTag& file_tag);

  ~Fetcher();

  void Setup() override;

 private:
  // Downloads and saves the requeted file to the given path
  size_t DownloadAndSaveFile(const std::string& path,
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
};

class FetcherFactory : public server::FetcherFactory {
 public:
  FetcherFactory(TokenSource& token_source) : token_source_(token_source) {}

  std::unique_ptr<server::Fetcher> Create(
      const proto::PublicFileTag& file_tag) override {
    return std::unique_ptr<server::Fetcher>{
        new Fetcher{token_source_, file_tag}};
  }

 private:
  TokenSource& token_source_;
};
}
}
}
