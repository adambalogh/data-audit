#include "audit/client/upload/storage.h"

#include <fstream>
#include <sstream>

#include "cryptopp/aes.h"
#include "cryptopp/modes.h"

#include "audit/settings.h"

using CryptoPP::AES;
using CryptoPP::SecByteBlock;

namespace audit {
namespace upload {

void Storage::StoreFile(const std::string& file_name, std::istream& stream,
                        StorageListener& listener) {
  if (!stream) {
    throw std::runtime_error{"Given stream for file '" + file_name +
                             "' is not open"};
  }
  file_storage_->StoreFile(GetFilePath(file_name), stream, listener);
}

void Storage::StoreFileTag(const std::string& file_name,
                           const proto::PrivateFileTag& file_tag,
                           StorageListener& listener) {
  Settings s;
  auto key_str = s.Get<std::string>("key");
  SecByteBlock key(reinterpret_cast<const unsigned char*>(key_str.data()),
                   key_str.size());

  auto iv_str = s.Get<std::string>("iv");
  unsigned char iv[AES::BLOCKSIZE];
  std::memcpy(&iv[0], iv_str.data(), iv_str.size());

  CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption(key, key.size(), iv);
  auto bin = file_tag.SerializeAsString();
  encryption.ProcessData(reinterpret_cast<unsigned char*>(&bin[0]),
                         reinterpret_cast<unsigned char*>(&bin[0]), bin.size());

  std::stringstream stream{bin};
  file_storage_->StoreFile(GetFileTagPath(file_name), stream, listener);
}

void Storage::StoreBlockTagFile(const std::string& file_name,
                                const std::string& block_file_path,
                                StorageListener& listener) {
  std::ifstream block_tag_file{block_file_path, std::ifstream::binary};
  if (!block_tag_file) {
    throw std::runtime_error("Could not open file containing BlockTags (" +
                             block_file_path + ")");
  }
  file_storage_->StoreFile(GetBlockTagFilePath(file_name), block_tag_file,
                           listener);
}
}
}
