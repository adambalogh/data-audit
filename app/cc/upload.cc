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
#include "audit/providers/dropbox/storage.h"

using namespace audit;

using audit::dropbox::Storage;
using audit::dropbox::TokenSource;
using audit::upload::ReusableStorage;
using audit::upload::Client;

using v8::Local;
using v8::Value;
using v8::Function;
using v8::FunctionTemplate;
using v8::Integer;
using v8::String;

using Nan::Callback;
using Nan::New;

// TODO find a better way for this
Client* client;

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

    upload::File file{content, GetFileName()};

    try {
      auto stats = client->Upload(file, [&execution_progress](int percentage) {
        execution_progress.Send(reinterpret_cast<const char*>(&percentage),
                                sizeof(int));
      });

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
    Local<Value> argv[] = {
        New<Integer>(*reinterpret_cast<int*>(const_cast<char*>(data)))};
    progress_bar_callback_->Call(1, argv);
  }

 private:
  std::string GetFileName() {
    return file_path_.substr(file_path_.find_last_of("\\/") + 1);
  }

  Callback* const progress_bar_callback_;

  const std::string file_path_;
};

NAN_METHOD(Upload) {
  if (info.Length() != 3) {
    Nan::ThrowError("The number of arguments must be 3.");
    return;
  }
  String::Utf8Value param1(info[0]->ToString());
  std::string file_path{*param1};

  Callback* progress_bar_callback = new Callback(info[1].As<Function>());
  Callback* callback = new Callback(info[2].As<Function>());

  AsyncQueueWorker(
      new UploadWorker(callback, progress_bar_callback, file_path));
}

NAN_MODULE_INIT(InitAll) {
  TokenSource token_source_{[]() {
    std::string code;
    std::cin >> code;
    return code;
  }};

  Client client_{std::unique_ptr<ReusableStorage>{new Storage{token_source_}}};
  client = &client_;

  Nan::Set(target, New<String>("uploadAsync").ToLocalChecked(),
           Nan::GetFunction(New<FunctionTemplate>(Upload)).ToLocalChecked());
}

NODE_MODULE(upload, InitAll)
