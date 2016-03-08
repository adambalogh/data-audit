#pragma once

#include "audit/client/upload/progress_bar.h"

namespace audit {
namespace upload {

// Interface for objects whose progress can be tracked
//
class Progressable {
 public:
  Progressable(ProgressBar& progress_bar) : progress_bar_(progress_bar) {}

  virtual ~Progressable() {}

  void Progress(int n) { progress_bar_.Progress(n); }

 private:
  ProgressBar& progress_bar_;
};
}
}
