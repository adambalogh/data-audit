#pragma once

#include <nan.h>

#include <memory>

#include "provider.h"

#include "audit/files/file_browser.h"

using v8::Value;
using v8::Local;
using v8::Function;
using v8::Object;
using v8::String;
using v8::Array;

using Nan::FunctionCallbackInfo;
using Nan::MakeCallback;
using Nan::New;

NAN_METHOD(GetFiles);
NAN_METHOD(RefreshFiles);

static std::unique_ptr<audit::file_browser::FileBrowser> fb;

void RefreshFiles(const FunctionCallbackInfo<Value>& info) {
  fb.reset(nullptr);
  return GetFiles(info);
}

void GetFiles(const FunctionCallbackInfo<Value>& info) {
  if (!fb) {
    fb.reset(
        new audit::file_browser::FileBrowser{std::move(GetFileListSource())});
  }

  Local<Function> cb;
  std::vector<audit::file_browser::File> files;

  try {
    if (info.Length() == 2) {
      String::Utf8Value param1{info[0]->ToString()};
      std::string query{*param1};

      cb = info[1].As<Function>();
      files = fb->Search(query);
    } else {
      cb = info[0].As<Function>();
      files = fb->GetAllFiles();
    }
  } catch (std::exception& e) {
    std::cout << e.what();
  }

  Local<Array> output = New<Array>(files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    Local<Array> file = New<Array>(2);
    file->Set(0, New<String>(files[i].name).ToLocalChecked());
    file->Set(1, New<String>(files[i].Size()).ToLocalChecked());

    output->Set(i, file);
  }

  const unsigned argc = 1;
  Local<Value> argv[argc] = {output};

  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}
