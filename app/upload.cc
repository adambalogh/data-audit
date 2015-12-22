#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include <util.h>
#include <audit/client/tagger.h>
#include <audit/client/local_disk_storage.h>
#include <audit/client/prf.h>
#include <audit/client/verification.h>

std::string GetFileName(const std::string& file_path) {
  return file_path.substr(file_path.find_last_of("\\/") + 1);
}

void Upload(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::String::Utf8Value param1(info[0]->ToString());

  std::string file_path{*param1};
  auto file_name = GetFileName(file_path);

  audit::Tagger tagger{
      std::unique_ptr<audit::Storage>{new audit::LocalDiskStorage{file_name}},
      std::unique_ptr<audit::RandomNumberGenerator>{
          new audit::CryptoNumberGenerator},
      std::unique_ptr<audit::PRF>{new audit::HMACPRF}};

  std::ifstream file{file_path};
  if (!file.is_open()) {
    Nan::ThrowError("File could not be opened.");
  }

  tagger.Run(file, file_name);

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
