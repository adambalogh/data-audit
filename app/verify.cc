#include <fstream>
#include <memory>
#include <string>
#include <iostream>

#include <nan.h>

#include "audit/util.h"
#include "audit/client/verify/client.h"
#include "audit/client/verify/local_disk_file_tag_source.h"
#include "audit/client/verify/local_proof_source.h"

void Verify(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::String::Utf8Value param1(info[0]->ToString());

  std::string file_path{*param1};

  audit::verify::Client client{std::unique_ptr<audit::verify::FileTagSource>(
                                   new audit::verify::LocalDiskFileTagSource{}),
                               std::unique_ptr<audit::verify::ProofSource>(
                                   new audit::verify::LocalProofSource)};

  bool result = client.Verify(file_path, 100);

  v8::Local<v8::Function> cb = info[1].As<v8::Function>();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = {Nan::New(result)};
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("verify").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Verify)->GetFunction());
}

NODE_MODULE(verify, Init)
