#include "audit/client/verify/file_tag_source.h"

#include "cryptopp/aes.h"
#include "cryptopp/modes.h"

#include "audit/settings.h"

using CryptoPP::AES;
using CryptoPP::SecByteBlock;

namespace audit {
namespace verify {

proto::PrivateFileTag FileTagSource::GetFileTag(const std::string& file_name) {
  Settings s;
  auto key_str = s.Get<std::string>("key");
  SecByteBlock key(reinterpret_cast<const unsigned char*>(key_str.data()),
                   key_str.size());

  auto iv_str = s.Get<std::string>("iv");
  unsigned char iv[AES::BLOCKSIZE];
  std::memcpy(&iv[0], iv_str.data(), iv_str.size());

  CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption(key, key.size(), iv);

  auto bin = source_->GetFileTag(file_name);
  decryption.ProcessData(reinterpret_cast<unsigned char*>(&bin[0]),
                         reinterpret_cast<unsigned char*>(&bin[0]), bin.size());

  proto::PrivateFileTag tag;
  tag.ParseFromArray(bin.data(), bin.size());
  return tag;
}
}
}
