#include "verify.h"

#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "audit/util.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/no_server_proof_source.h"
#include "audit/providers/dropbox/fetcher.h"
#include "audit/providers/dropbox/file_tag_source.h"
#include "audit/providers/dropbox/token_source.h"

using namespace audit;

using audit::verify::Client;
using audit::verify::FileTagSource;
using audit::verify::ProofSource;
using audit::dropbox::TokenSourceInstance;

using v8::Integer;
using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::New;
using Nan::HandleScope;

static std::unique_ptr<Client> client;

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
      result_ = client->Verify(file_name_, 100);
    } catch (std::exception& e) {
      SetErrorMessage(e.what());
    }
  }

  void HandleErrorCallback() override {
    ::HandleScope scope;
    Local<Value> argv[] = {
        New(false),
        v8::Exception::Error(New<String>(ErrorMessage()).ToLocalChecked())};
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

  if (!client) {
    client.reset(new Client{
        std::unique_ptr<FileTagSource>(
            new dropbox::FileTagSource{TokenSourceInstance::Get()}),
        std::unique_ptr<ProofSource>(new verify::NoServerProofSource{
            std::unique_ptr<server::FetcherFactory>{
                new dropbox::FetcherFactory{TokenSourceInstance::Get()}}})});
  }

  String::Utf8Value param1(info[0]->ToString());
  std::string file_name{*param1};

  Callback* progress_bar_callback = new Callback(info[1].As<Function>());
  Callback* callback = new Callback(info[2].As<Function>());

  AsyncQueueWorker(
      new VerifyWorker(callback, progress_bar_callback, file_name));
}
