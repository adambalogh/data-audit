#define _TURN_OFF_PLATFORM_STRING

#include <nan.h>

#include "provider.h"
#include "login.h"
#include "upload.h"
#include "verify.h"
#include "file_browser.h"
#include "recent_files.h"
#include "settings.h"

using Nan::New;

using v8::Local;
using v8::Value;
using v8::FunctionTemplate;
using v8::String;

NAN_MODULE_INIT(InitAll) {
  if (RAND_load_file("/dev/urandom", 128) != 128) {
    throw std::runtime_error("Could not seed RAND");
  }

#if PROVIDER_TYPE == dropbox
  Nan::Set(target, New<String>("getAuthorizeUrl").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(GetAuthorizeUrl))
               .ToLocalChecked());
  Nan::Set(target, New<String>("exchangeCodeForToken").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(ExchangeCodeForToken))
               .ToLocalChecked());
#endif
  //

  Nan::Set(
      target, New<String>("hasToLogin").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(HasToLogin)).ToLocalChecked());

  Nan::Set(target, New<String>("uploadAsync").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Upload)).ToLocalChecked());

  Nan::Set(target, New<String>("verifyAsync").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Verify)).ToLocalChecked());

  Nan::Set(target, New<String>("getFiles").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(GetFiles)).ToLocalChecked());
  Nan::Set(
      target, New<String>("getRecentFiles").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(GetRecentFiles)).ToLocalChecked());
  Nan::Set(
      target, New<String>("refreshFiles").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(RefreshFiles)).ToLocalChecked());
  Nan::Set(
      target, New<String>("settingsGet").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(SettingsGet)).ToLocalChecked());
  Nan::Set(
      target, New<String>("settingsSet").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(SettingsSet)).ToLocalChecked());
}

NODE_MODULE(native, InitAll)
