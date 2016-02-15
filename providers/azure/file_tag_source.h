#pragma once

#include "cpprest/http_client.h"

#include "audit/client/verify/file_tag_source.h"
#include "audit/proto/cpor.pb.h"
#include "audit/providers/azure/urls.h"

namespace audit {
namespace providers {
namespace azure {

class FileTagSource : public verify::FileTagSource {
 public:
  proto::PrivateFileTag GetFileTag(const std::string& file_name) override;

 private:
  web::http::client::http_client client_{BASE_URL};
};
}
}
}
