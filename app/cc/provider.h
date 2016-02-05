#pragma once

// clang-format off

// Change provider here to dropbox, local_disk etc...
#define PROVIDER_TYPE dropbox

#define PROVIDER_METHOD(provider, name) provider ## name

#define EVAL(a, b) PROVIDER_METHOD(a, b)
#define GET_PROVIDER_METHOD(name) EVAL(PROVIDER_TYPE, name)

#define GetStorage GET_PROVIDER_METHOD(GetStorage)
#define GetFetcherFactory GET_PROVIDER_METHOD(GetFetcherFactory)
#define GetFileListSource GET_PROVIDER_METHOD(GetFileListSource)
#define GetFileTagSource GET_PROVIDER_METHOD(GetFileTagSource)

// clang-format on

#include <memory>

#include "audit/client/upload/storage.h"
#include "audit/client/verify/file_tag_source.h"
#include "audit/files/file_list_source.h"
#include "audit/server/fetcher.h"

#include "audit/providers/dropbox/storage.h"
#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_list_source.h"
#include "audit/providers/dropbox/file_tag_source.h"
#include "audit/providers/dropbox/token_source.h"

using audit::upload::ReusableStorage;
using audit::verify::FileTagSource;
using audit::server::FetcherFactory;
using audit::FileListSource;

using audit::providers::dropbox::TokenSourceInstance;

std::unique_ptr<ReusableStorage> PROVIDER_METHOD(dropbox, GetStorage)() {
  return std::unique_ptr<ReusableStorage>(
      new audit::providers::dropbox::Storage{TokenSourceInstance::Get()});
}

std::unique_ptr<FetcherFactory> PROVIDER_METHOD(dropbox, GetFetcherFactory)() {
  return std::unique_ptr<FetcherFactory>(
      new audit::providers::dropbox::FetcherFactory{
          TokenSourceInstance::Get()});
}

std::unique_ptr<FileListSource> PROVIDER_METHOD(dropbox, GetFileListSource)() {
  return std::unique_ptr<FileListSource>(
      new audit::providers::dropbox::FileListSource{
          TokenSourceInstance::Get()});
}

std::unique_ptr<FileTagSource> PROVIDER_METHOD(dropbox, GetFileTagSource)() {
  return std::unique_ptr<FileTagSource>(
      new audit::providers::dropbox::FileTagSource{TokenSourceInstance::Get()});
}
