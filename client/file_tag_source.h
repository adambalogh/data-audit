#pragma once

class FileTagSource {
 public:
  virtual proto::PrivateFileTag GetFileTag(const std::string& file_name) = 0;
};
