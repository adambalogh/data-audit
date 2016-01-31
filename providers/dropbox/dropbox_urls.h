#pragma once

#include "cpprest/uri.h"

namespace audit {
namespace dropbox {

const web::uri AUTHORIZE_URL{"https://www.dropbox.com/1/oauth2/authorize"};

const web::uri BASE_URL{"https://content.dropboxapi.com"};

const web::uri DOWNLOAD_PATH{"/2/files/download"};

// v2 doesn't support Range HTTP header
const web::uri V1_DOWNLOAD_PATH{"/1/files/auto/"};

const web::uri UPLOAD_PATH{"/2/files/upload"};

const web::uri AUTH_URL{"https://api.dropboxapi.com"};

const web::uri TOKEN_PATH{"/1/oauth2/token"};
}
}
