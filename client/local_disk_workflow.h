#pragma once

#include <string>

#include "audit/client/workflow.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

class LocalDiskWorkFlow : public WorkflowWithFileName {
 public:
  LocalDiskWorkFlow(const std::string& file_name)
      : WorkflowWithFileName(file_name) {}

  virtual void ProcessBlockTag(const proto::BlockTag& tag) override;
  virtual void Finish(const proto::PrivateFileTag& file_tag) override;

 private:
  std::string file_dir_{"/users/adambalogh/Developer/audit/files_dir/"};
};
}
