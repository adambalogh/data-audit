#pragma once

#include "cpprest/uri.h"

namespace audit {
namespace providers {
namespace azure {

// Not valid
const web::uri BASE_URL{""};

const web::uri UPLOAD_URL{"/upload"};

const web::uri PROVE_URL{"/prove"};

const web::uri BATCH_PROVE_URL{"/batch_prove"};

const web::uri FILETAG_URL{"/filetag"};

const web::uri FILELIST_URL{"/filelist"};
}
}
}
