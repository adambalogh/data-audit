#pragma once

// clang-format off

// Change provider here to dropbox, local_disk etc...
#define PROVIDER_TYPE dropbox

// Generates an unique method name for a given provider, e.g.
// PROVIDER_METHOD(dropbox, Hello) = dropboxHello
#define PROVIDER_METHOD(provider, name) provider ## name

#define EVAL(a, b) PROVIDER_METHOD(a, b)
#define GET_PROVIDER_METHOD(name) EVAL(PROVIDER_TYPE, name)

#define GetStorage GET_PROVIDER_METHOD(GetStorage)
#define GetFetcherFactory GET_PROVIDER_METHOD(GetFetcherFactory)
#define GetFileListSource GET_PROVIDER_METHOD(GetFileListSource)
#define GetFileTagSource GET_PROVIDER_METHOD(GetFileTagSource)
#define GetAuthorizationNeeded GET_PROVIDER_METHOD(GetAuthorizationNeeded)

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
#include "audit/providers/dropbox/token_source_instance.h"

#include "audit/providers/local_disk/storage.h"
#include "audit/providers/local_disk/fetcher.h"
#include "audit/providers/local_disk/file_list_source.h"
#include "audit/providers/local_disk/file_tag_source.h"

using audit::upload::ReusableStorage;
using audit::verify::FileTagSource;
using audit::server::FetcherFactory;
using audit::FileListSource;

using audit::providers::dropbox::TokenSourceInstance;

// TODO make these factories

// Dropbox Methods
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

bool PROVIDER_METHOD(dropbox, GetAuthorizationNeeded)() {
  return TokenSourceInstance::Get().NeedToAuthorize();
}

// Local Disk methods
std::unique_ptr<ReusableStorage> PROVIDER_METHOD(local_disk, GetStorage)() {
  return std::unique_ptr<ReusableStorage>(
      new audit::providers::local_disk::Storage);
}

std::unique_ptr<FetcherFactory> PROVIDER_METHOD(local_disk,
                                                GetFetcherFactory)() {
  return std::unique_ptr<FetcherFactory>(
      new audit::providers::local_disk::FetcherFactory);
}

std::unique_ptr<FileListSource> PROVIDER_METHOD(local_disk,
                                                GetFileListSource)() {
  return std::unique_ptr<FileListSource>(
      new audit::providers::local_disk::FileListSource);
}

std::unique_ptr<FileTagSource> PROVIDER_METHOD(local_disk, GetFileTagSource)() {
  return std::unique_ptr<FileTagSource>(
      new audit::providers::local_disk::FileTagSource);
}

bool PROVIDER_METHOD(local_disk, GetAuthorizationNeeded)() { return false; }
