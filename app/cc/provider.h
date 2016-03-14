#pragma once

// clang-format off

// Change provider here to dropbox, local_disk etc...
#define PROVIDER_TYPE dropbox

// Generates an unique method name for a given provider, e.g.
// PROVIDER_METHOD(dropbox, Hello) = dropboxHello
#define PROVIDER_METHOD(provider, name) provider ## name

#define EVAL(a, b) PROVIDER_METHOD(a, b)
#define GET_PROVIDER_METHOD(name) EVAL(PROVIDER_TYPE, name)

#define GetFileStorage GET_PROVIDER_METHOD(GetFileStorage)
#define GetProofSource GET_PROVIDER_METHOD(GetProofSource)
#define GetFileListSource GET_PROVIDER_METHOD(GetFileListSource)
#define GetFileTagSource GET_PROVIDER_METHOD(GetFileTagSource)
#define GetAuthorizationNeeded GET_PROVIDER_METHOD(GetAuthorizationNeeded)

// clang-format on

#include <memory>

#include "audit/client/upload/storage.h"
#include "audit/client/verify/file_tag_source.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/files/file_list_source.h"
#include "audit/server/fetcher.h"

#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_list_source.h"
#include "audit/providers/dropbox/file_storage.h"
#include "audit/providers/dropbox/file_tag_source.h"
#include "audit/providers/dropbox/token_source.h"
#include "audit/providers/dropbox/token_source_instance.h"

#include "audit/providers/local_disk/file_storage.h"
#include "audit/providers/local_disk/fetcher.h"
#include "audit/providers/local_disk/file_list_source.h"
#include "audit/providers/local_disk/file_tag_source.h"

#include "audit/providers/azure/file_storage.h"
#include "audit/providers/azure/proof_source.h"
#include "audit/providers/azure/file_tag_source.h"
#include "audit/providers/azure/file_list_source.h"

using audit::upload::FileStorage;
using audit::verify::BinaryFileTagSource;
using audit::file_browser::FileListSource;
using audit::verify::ProofSource;

// TODO make these factories

// Dropbox
std::unique_ptr<FileStorage> PROVIDER_METHOD(dropbox, GetFileStorage)() {
  return std::make_unique<audit::providers::dropbox::FileStorage>(
      audit::providers::dropbox::TokenSourceInstance::Get());
}

std::unique_ptr<ProofSource> PROVIDER_METHOD(dropbox, GetProofSource)() {
  return std::make_unique<audit::verify::NoServerProofSource>(
      std::make_unique<audit::providers::dropbox::FetcherFactory>(
          audit::providers::dropbox::TokenSourceInstance::Get()));
}

std::unique_ptr<FileListSource> PROVIDER_METHOD(dropbox, GetFileListSource)() {
  return std::make_unique<audit::providers::dropbox::FileListSource>(
      audit::providers::dropbox::TokenSourceInstance::Get());
}

std::unique_ptr<BinaryFileTagSource> PROVIDER_METHOD(dropbox,
                                                     GetFileTagSource)() {
  return std::make_unique<audit::providers::dropbox::FileTagSource>(
      audit::providers::dropbox::TokenSourceInstance::Get());
}

bool PROVIDER_METHOD(dropbox, GetAuthorizationNeeded)() {
  return audit::providers::dropbox::TokenSourceInstance::Get()
      .NeedToAuthorize();
}

// Local Disk methods
std::unique_ptr<FileStorage> PROVIDER_METHOD(local_disk, GetFileStorage)() {
  return std::make_unique<audit::providers::local_disk::FileStorage>();
}

std::unique_ptr<ProofSource> PROVIDER_METHOD(local_disk, GetProofSource)() {
  return std::make_unique<audit::verify::NoServerProofSource>(
      std::make_unique<audit::providers::local_disk::FetcherFactory>());
}

std::unique_ptr<FileListSource> PROVIDER_METHOD(local_disk,
                                                GetFileListSource)() {
  return std::make_unique<audit::providers::local_disk::FileListSource>();
}

std::unique_ptr<BinaryFileTagSource> PROVIDER_METHOD(local_disk,
                                                     GetFileTagSource)() {
  return std::make_unique<audit::providers::local_disk::FileTagSource>();
}

bool PROVIDER_METHOD(local_disk, GetAuthorizationNeeded)() { return false; }

// Azure methods
std::unique_ptr<FileStorage> PROVIDER_METHOD(azure, GetFileStorage)() {
  return std::make_unique<audit::providers::azure::FileStorage>();
}

std::unique_ptr<ProofSource> PROVIDER_METHOD(azure, GetProofSource)() {
  return std::make_unique<audit::providers::azure::ProofSource>();
}

std::unique_ptr<FileListSource> PROVIDER_METHOD(azure, GetFileListSource)() {
  return std::make_unique<audit::providers::azure::FileListSource>();
}

std::unique_ptr<BinaryFileTagSource> PROVIDER_METHOD(azure,
                                                     GetFileTagSource)() {
  return std::make_unique<audit::providers::azure::FileTagSource>();
}

bool PROVIDER_METHOD(azure, GetAuthorizationNeeded)() { return false; }
