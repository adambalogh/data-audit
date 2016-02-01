#define _TURN_OFF_PLATFORM_STRING

#include <nan.h>

#include "login.h"
#include "upload.h"
#include "verify.h"
#include "file_browser.h"

using v8::Local;
using v8::Value;
using v8::FunctionTemplate;
using v8::String;

using Nan::New;

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, New<String>("login").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Login)).ToLocalChecked());
  Nan::Set(target, New<String>("uploadAsync").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Upload)).ToLocalChecked());

  Nan::Set(target, New<String>("verifyAsync").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Verify)).ToLocalChecked());

  Nan::Set(target, New<String>("getFiles").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(GetFiles)).ToLocalChecked());
  Nan::Set(
      target, New<String>("refreshFiles").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(RefreshFiles)).ToLocalChecked());
}

NODE_MODULE(native, InitAll)
