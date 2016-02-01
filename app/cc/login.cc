#include "login.h"

#include <nan.h>

#include <iostream>

#include "audit/providers/dropbox/token_source.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::New;

using audit::dropbox::TokenSourceInstance;

void LoginImpl(const Nan::FunctionCallbackInfo<Value>& info) {
  TokenSourceInstance::Get().Initialize([&info]() {
    Callback* callback = new Callback(info[0].As<Function>());
    auto response = callback->Call(0, NULL);

    String::Utf8Value utf{response};
    std::string token{*utf};

    return token;
  });
  std::cout << &TokenSourceInstance::Get() << std::endl;
}

NAN_METHOD(Login) { LoginImpl(info); }
