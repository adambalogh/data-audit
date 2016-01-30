#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "verify.h"

#include "audit/util.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/local_proof_source.h"
#include "audit/providers/local_disk/local_disk_file_tag_source.h"

audit::verify::Client client{std::unique_ptr<audit::verify::FileTagSource>(
                                 new audit::verify::LocalDiskFileTagSource{}),
                             std::unique_ptr<audit::verify::ProofSource>(
                                 new audit::verify::LocalProofSource)};

class VerifyWorker : public Nan::AsyncProgressWorker {
 public:
  VerifyWorker(Nan::Callback* callback, Nan::Callback* progress_bar_callback,
               const std::string& file_name)
      : AsyncProgressWorker(callback),
        progress_bar_callback_(progress_bar_callback),
        file_name_(file_name) {}

  void Execute(const AsyncProgressWorker::ExecutionProgress& execution_progress)
      override {
    try {
      result_ = client.Verify(file_name_, 100);
    } catch (std::exception& e) {
      SetErrorMessage(e.what());
    }
  }

  void HandleErrorCallback() override {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {
        Nan::New(false),
        v8::Exception::Error(
            Nan::New<v8::String>(ErrorMessage()).ToLocalChecked())};
    callback->Call(2, argv);
  }

  void HandleOKCallback() override {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {Nan::New(result_), Nan::Null()};
    callback->Call(2, argv);
  }

  // This is called everytime the verification makes progress
  void HandleProgressCallback(const char* data, size_t size) override {
    v8::Local<v8::Value> argv[] = {Nan::New<v8::Integer>(
        *reinterpret_cast<int*>(const_cast<char*>(data)))};
    progress_bar_callback_->Call(1, argv);
  }

 private:
  Nan::Callback* const progress_bar_callback_;

  const std::string file_name_;

  bool result_;
};

NAN_METHOD(Verify) {
  if (info.Length() != 3) {
    Nan::ThrowError("The number of arguments must be 3.");
    return;
  }

  v8::String::Utf8Value param1(info[0]->ToString());
  std::string file_name{*param1};

  Nan::Callback* progress_bar_callback =
      new Nan::Callback(info[1].As<v8::Function>());
  Nan::Callback* callback = new Nan::Callback(info[2].As<v8::Function>());

  AsyncQueueWorker(
      new VerifyWorker(callback, progress_bar_callback, file_name));
}

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New<v8::String>("verifyAsync").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Verify))
               .ToLocalChecked());
}

NODE_MODULE(verify, InitAll)
