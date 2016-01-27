#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "upload.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/client/upload/local_disk_storage.h"

audit::upload::Client client{std::unique_ptr<audit::upload::Storage>{
    new audit::upload::LocalDiskStorage}};

class UploadWorker : public Nan::AsyncWorker {
 public:
  UploadWorker(Nan::Callback* callback, const std::string& file_path)
      : AsyncWorker(callback), file_path_(file_path) {}

  void Execute() override {
    std::ifstream content{file_path_};

    // TODO use full file path for file name
    audit::upload::File file{content, GetFileName()};

    try {
      client.Upload(file);
    } catch (std::runtime_error& e) {
      std::string error = "Runtime error: " + std::string(e.what());
      SetErrorMessage(error.c_str());
    } catch (std::exception& e) {
      std::string error = e.what();
      SetErrorMessage(error.c_str());
    }
  }

  void HandleOKCallback() override {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {Nan::Null()};
    callback->Call(1, argv);
  }

 private:
  std::string GetFileName() {
    return file_path_.substr(file_path_.find_last_of("\\/") + 1);
  }

  const std::string file_path_;
};

NAN_METHOD(Upload) {
  v8::String::Utf8Value param1(info[0]->ToString());
  std::string file_path{*param1};
  Nan::Callback* callback = new Nan::Callback(info[1].As<v8::Function>());

  AsyncQueueWorker(new UploadWorker(callback, file_path));
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New<v8::String>("uploadAsync").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Upload))
               .ToLocalChecked());
}

NODE_MODULE(upload, InitAll)
