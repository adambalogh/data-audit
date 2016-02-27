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

NAN_METHOD(SettingsGetParams) {
  Settings s;
  auto val = s.Get<std::string>("params");
  auto cb = info[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = {New(val).ToLocalChecked()};
  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

NAN_METHOD(SettingsGetVerificationPercentage) {
  Settings s;
  auto val = s.Get<int>("verification_percentage");
  auto cb = info[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = {New(val)};
  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

NAN_METHOD(SettingsRecommended) {
  Settings s;
  s.Set("num_sectors", 35);
  s.Set("sector_size", 96);
  s.Set("params", "recommended");
}

NAN_METHOD(SettingsMinStorage) {
  Settings s;
  s.Set("num_sectors", 100);
  s.Set("sector_size", 96);
  s.Set("params", "min_storage");
}

NAN_METHOD(SettingsVerification) {
  Local<Integer> val = info[0].As<Integer>();
  Settings s;
  s.Set("verification_percentage", val->Int32Value());
}

NAN_METHOD(SettingsProvider) {
  String::Utf8Value param1(info[0]->ToString());
  std::string provider{*param1};

  Settings s;
  s.Set("provider", provider);
}
