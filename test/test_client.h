#pragma once

#include <fstream>

#include "audit/proto/cpor.pb.h"

using namespace audit;

class TestClient {
 public:
  TestClient(const std::string& content);
  ~TestClient();

  void Run();

 private:
  std::vector<std::string> files_to_delete;

  const std::string file_path_ = "/users/adambalogh/Developer/audit/files_dir/";
  const std::string file_name_ = "test_file";
  const std::string full_file_name_ = file_path_ + file_name_;

  proto::PublicFileTag file_tag;
  proto::PrivateFileTag private_file_tag;
};
