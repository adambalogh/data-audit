#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/client/upload/local_disk_storage.h"

std::string GetFileName(const std::string& file_path) {
  return file_path.substr(file_path.find_last_of("\\/") + 1);
}

void Upload(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::String::Utf8Value param1(info[0]->ToString());

  std::string file_path{*param1};
  auto file_name = GetFileName(file_path);

  audit::upload::Client client{std::unique_ptr<audit::upload::Storage>{
      new audit::upload::LocalDiskStorage}};

  std::ifstream content{file_path};
  // TODO use full file path
  audit::upload::File file{content, file_name};

  client.Upload(file);

  v8::Local<v8::Function> cb = info[1].As<v8::Function>();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = {Nan::New(file_name).ToLocalChecked()};
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("upload").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Upload)->GetFunction());
}

NODE_MODULE(upload, Init)
