#pragma once

#include <string>

#include "audit/proto/cpor.pb.h"

namespace audit {

class Workflow {
 public:
  virtual void ProcessBlockTag(const proto::BlockTag& tag) = 0;
  virtual void Finish(const proto::PrivateFileTag& file_tag) = 0;

  virtual ~Workflow() {}
};

class WorkflowWithFileName : public Workflow {
 public:
  WorkflowWithFileName(const std::string& file_name) : file_name_(file_name) {}

 protected:
  std::string file_name_;
};
}
