#pragma once

// clang-format off

// Change provider here to dropbox, local_disk etc...
#define PROVIDER_TYPE dropbox

#define GetStorage dropboxGetStorage
#define GetFetcherFactory dropboxGetFetcherFactory
#define GetFileListSource dropboxGetFileListSource
#define GetFileTagSource dropboxGetFileTagSource

// clang-format on

#include <memory>

#include "audit/client/upload/storage.h"
#include "audit/client/verify/file_tag_source.h"
#include "audit/server/fetcher.h"
#include "audit/files/file_list_source.h"

#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_list_source.h"
#include "audit/providers/dropbox/file_tag_source.h"

using audit::upload::ReusableStorage;
using audit::verify::FileTagSource;
using audit::server::FetcherFactory;
using audit::FileListSource;

using audit::providers::dropbox::TokenSourceInstance;

std::unique_ptr<ReusableStorage> dropboxGetStorage() {
  return std::unique_ptr<ReusableStorage>(
      new audit::providers::dropbox::Storage{TokenSourceInstance::Get()});
}

std::unique_ptr<FetcherFactory> dropboxGetFetcherFactory() {
  return std::unique_ptr<FetcherFactory>(
      new audit::providers::dropbox::FetcherFactory{
          TokenSourceInstance::Get()});
}

std::unique_ptr<FileListSource> dropboxGetFileListSource() {
  return std::unique_ptr<FileListSource>(
      new audit::providers::dropbox::FileListSource{
          TokenSourceInstance::Get()});
}

std::unique_ptr<FileTagSource> dropboxGetFileTagSource() {
  return std::unique_ptr<FileTagSource>(
      new audit::providers::dropbox::FileTagSource{TokenSourceInstance::Get()});
}
