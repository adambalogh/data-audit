#pragma once

#include <iostream>

#include "CPORTypes.h"

namespace audit {

class FileProcessor {
 private:
  BlockTag GenerateTag(Block& block) const;
};
}
