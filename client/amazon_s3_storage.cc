#include "audit/client/amazon_s3_storage.h"

namespace audit {

void AmazonS3Storage::StoreBlockTag(const proto::BlockTag& tag) {}

void AmazonS3Storage::StoreFileTag(const proto::PrivateFileTag& file_tag) {}

void AmazonS3Storage::StoreFile(std::istream& file) {}
}
