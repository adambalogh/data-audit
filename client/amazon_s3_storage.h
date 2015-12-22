#pragma once

#include <istream>

#include <aws/s3/S3Client>

#include "audit/client/storage.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

class AmazonS3Storage : public Storage {
 public:
  AmazonS3Storage(const Aws::Auth::AWSCredentials& credentials)
      : client_(credentials) {}

  virtual void StoreBlockTag(const proto::BlockTag& tag);
  virtual void StoreFileTag(const proto::PrivateFileTag& file_tag);
  virtual void StoreFile(std::istream& file);

 private:
  Aws::S3::S3Client client_;
};
}
