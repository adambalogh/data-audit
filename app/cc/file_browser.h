#pragma once

#include <nan.h>

#include <memory>

#include "provider.h"
#include "audit/files/file_browser.h"
#include "audit/files/file_list_source.h"

using namespace audit;

using audit::providers::dropbox::TokenSourceInstance;

using v8::Value;
using v8::Local;
using v8::Function;
using v8::String;
using v8::Array;

using Nan::FunctionCallbackInfo;
using Nan::MakeCallback;
using Nan::New;

NAN_METHOD(GetFiles);
NAN_METHOD(RefreshFiles);

static std::unique_ptr<audit::FileBrowser> file_browser;

void RefreshFiles(const FunctionCallbackInfo<Value>& info) {
  file_browser.reset(nullptr);
  return GetFiles(info);
}

void GetFiles(const FunctionCallbackInfo<Value>& info) {
  if (!file_browser) {
    file_browser.reset(new audit::FileBrowser{std::move(GetFileListSource())});
  }

  Local<Function> cb;
  std::vector<const std::string> files;

  try {
    if (info.Length() == 2) {
      String::Utf8Value param1{info[0]->ToString()};
      std::string query{*param1};

      cb = info[1].As<Function>();
      files = file_browser->Search(query);
    } else {
      cb = info[0].As<Function>();
      files = file_browser->GetAllFiles();
    }
  } catch (std::exception& e) {
    std::cout << e.what();
  }

  Local<Array> output = New<Array>(files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    output->Set(i, Nan::New(files[i]).ToLocalChecked());
  }

  const unsigned argc = 1;
  Local<Value> argv[argc] = {output};

  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}
