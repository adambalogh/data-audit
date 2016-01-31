#pragma once

#include <memory>
#include <string>

#include "openssl/bn.h"

namespace audit {

typedef std::unique_ptr<BIGNUM, decltype(&::BN_free)> BN_ptr;
typedef std::unique_ptr<BN_CTX, decltype(&::BN_CTX_free)> BN_CTX_ptr;

const std::string application_dir{
    "/Library/Application Support/abaloghDataAudit/"};
}
