#pragma once

#include "cpprest/uri.h"

namespace audit {
namespace providers {
namespace azure {

const web::uri BASE_URL{""};

const web::uri UPLOAD_URL{"/upload"};

const web::uri PROVE_URL{"/prove"};
}
}
}
