#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "provider.h"
#include "audit/util.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/stats.h"

using namespace audit;

using audit::verify::ProofSource;
using audit::verify::NoServerProofSource;

using v8::Integer;
using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::New;
using Nan::HandleScope;

NAN_METHOD(Verify);

static std::unique_ptr<verify::Client> verify_client;

class VerifyWorker : public Nan::AsyncProgressWorker {
 public:
  VerifyWorker(Callback* callback, Callback* progress_bar_callback,
               const std::string& file_name)
      : AsyncProgressWorker(callback),
        progress_bar_callback_(progress_bar_callback),
        file_name_(file_name) {}

  void Execute(const AsyncProgressWorker::ExecutionProgress& execution_progress)
      override {
    try {
      verify::Stats stats;
      result_ = verify_client->Verify(file_name_, 100, stats);

      std::cout << "Stats for verifying " << file_name_ << ": " << std::endl;
      std::cout << stats.to_string() << std::endl;
    } catch (std::exception& e) {
      auto error = std::string{e.what()};
      SetErrorMessage(error.c_str());
    }
  }

  void HandleErrorCallback() override {
    ::HandleScope scope;
    Local<Value> argv[] = {New(false),
                           New<String>(ErrorMessage()).ToLocalChecked()};
    callback->Call(2, argv);
  }

  void HandleOKCallback() override {
    HandleScope scope;
    Local<Value> argv[] = {New(result_), Nan::Null()};
    callback->Call(2, argv);
  }

  // This is called everytime the verification makes progress
  void HandleProgressCallback(const char* data, size_t size) override {
    Local<Value> argv[] = {
        New<Integer>(*reinterpret_cast<int*>(const_cast<char*>(data)))};
    progress_bar_callback_->Call(1, argv);
  }

 private:
  Callback* const progress_bar_callback_;

  const std::string file_name_;

  bool result_;
};

NAN_METHOD(Verify) {
  if (info.Length() != 3) {
    Nan::ThrowError("The number of arguments must be 3.");
    return;
  }

  if (!verify_client) {
    verify_client.reset(
        new verify::Client{GetFileTagSource(), GetProofSource()});
  }

  String::Utf8Value param1(info[0]->ToString());
  std::string file_name{*param1};

  Callback* progress_bar_callback = new Callback(info[1].As<Function>());
  Callback* callback = new Callback(info[2].As<Function>());

  AsyncQueueWorker(
      new VerifyWorker(callback, progress_bar_callback, file_name));
}
