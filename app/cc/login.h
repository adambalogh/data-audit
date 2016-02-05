#pragma once

#include <nan.h>

#include "provider.h"

NAN_METHOD(GetAuthorizeUrl);
NAN_METHOD(ExchangeCodeForToken);
