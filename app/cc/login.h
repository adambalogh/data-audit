#pragma once

#include <nan.h>

#include <iostream>

#include "provider.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::MakeCallback;
using Nan::GetCurrentContext;
using Nan::New;

NAN_METHOD(GetAuthorizeUrl);
NAN_METHOD(ExchangeCodeForToken);

void GetAuthorizeUrl(const Nan::FunctionCallbackInfo<Value>& info) {
  Local<Function> cb = info[0].As<Function>();

  const unsigned argc = 1;
  Local<Value> argv[argc] = {
      New(TokenSourceInstance::Get().GetAuthorizeUrl()).ToLocalChecked()};
  MakeCallback(GetCurrentContext()->Global(), cb, argc, argv);
}

void ExchangeCodeForToken(const Nan::FunctionCallbackInfo<Value>& info) {
  String::Utf8Value utf{info[0].As<String>()};
  std::string token{*utf};

  TokenSourceInstance::Get().ExchangeCodeForToken(token);

  Local<Function> cb = info[1].As<Function>();
  MakeCallback(GetCurrentContext()->Global(), cb, 0, NULL);
}
