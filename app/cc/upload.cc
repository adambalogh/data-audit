#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "upload.h"

#include "audit/util.h"
#include "audit/client/prf.h"
#include "audit/client/upload/client.h"
#include "audit/client/upload/stats.h"
#include "audit/providers/local_disk/local_disk_storage.h"

audit::upload::Client client{std::unique_ptr<audit::upload::ReusableStorage>{
    new audit::upload::LocalDiskStorage}};

class UploadWorker : public Nan::AsyncProgressWorker {
 public:
  UploadWorker(Nan::Callback* callback, Nan::Callback* progress_bar_callback,
               const std::string& file_path)
      : AsyncProgressWorker(callback),
        progress_bar_callback_(progress_bar_callback),
        file_path_(file_path) {}

  void Execute(const AsyncProgressWorker::ExecutionProgress& execution_progress)
      override {
    std::ifstream content{file_path_, std::ifstream::binary};

    // TODO use full file path for file name?
    audit::upload::File file{content, GetFileName()};

    try {
      auto stats = client.Upload(file, [&execution_progress](int percentage) {
        execution_progress.Send(reinterpret_cast<const char*>(&percentage),
                                sizeof(int));
      });

      // Write out stats to console
      std::cout << "Stats for: " + GetFileName() << std::endl;
      std::cout << stats.String();

    } catch (std::runtime_error& e) {
      std::string error = "Runtime error: " + std::string(e.what());
      SetErrorMessage(error.c_str());
    } catch (std::exception& e) {
      std::string error = "Error: " + std::string(e.what());
      SetErrorMessage(error.c_str());
    }
  }

  // This is called everytime Upload makes progress
  void HandleProgressCallback(const char* data, size_t size) override {
    v8::Local<v8::Value> argv[] = {Nan::New<v8::Integer>(
        *reinterpret_cast<int*>(const_cast<char*>(data)))};
    progress_bar_callback_->Call(1, argv);
  }

 private:
  std::string GetFileName() {
    return file_path_.substr(file_path_.find_last_of("\\/") + 1);
  }

  Nan::Callback* const progress_bar_callback_;

  const std::string file_path_;
};

NAN_METHOD(Upload) {
  if (info.Length() != 3) {
    Nan::ThrowError("The number of arguments must be 3.");
    return;
  }
  v8::String::Utf8Value param1(info[0]->ToString());
  std::string file_path{*param1};

  Nan::Callback* progress_bar_callback =
      new Nan::Callback(info[1].As<v8::Function>());
  Nan::Callback* callback = new Nan::Callback(info[2].As<v8::Function>());

  AsyncQueueWorker(
      new UploadWorker(callback, progress_bar_callback, file_path));
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New<v8::String>("uploadAsync").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Upload))
               .ToLocalChecked());
}

NODE_MODULE(upload, InitAll)
