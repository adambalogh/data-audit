#pragma once

// clang-format off

// Change provider here to dropbox, local_disk etc...
#define PROVIDER_TYPE dropbox

#define STR(s) #s
#define EXPAND_AND_STR(s) STR(s)
#define COMPONENT(c) EXPAND_AND_STR(audit/providers/PROVIDER_TYPE/c.h)

// clang-format on

#include COMPONENT(storage)
#include COMPONENT(fetcher)
#include COMPONENT(file_list_source)
#include COMPONENT(file_tag_source)

typedef audit::providers::PROVIDER_TYPE::Storage Storage_t;
typedef audit::providers::PROVIDER_TYPE::Fetcher Fetcher_t;
typedef audit::providers::PROVIDER_TYPE::FileListSource FileListSource_t;
typedef audit::providers::PROVIDER_TYPE::FileTagSource FileTagSource_t;
