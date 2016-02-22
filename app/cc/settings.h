#pragma once

#include "nan.h"

#include "audit/settings.h"

using v8::Array;
using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::MakeCallback;
using Nan::GetCurrentContext;
using Nan::New;
using Nan::FunctionCallbackInfo;

NAN_METHOD(SettingsGet) {
  Local<String> key = info[0].As<String>();

  Settings s;
  auto val = s.Get<std::string>(std::string{*String::Utf8Value{key}});

  auto cb = info[1].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = {New(val).ToLocalChecked()};
  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

NAN_METHOD(SettingsSet) {
  Local<String> key = info[0].As<String>();
  Local<String> value = info[1].As<String>();

  Settings s;
  s.Set(std::string{*String::Utf8Value(key)},
        std::string{*String::Utf8Value(value)});
}