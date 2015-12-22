#include <memory>

#include <nan.h>

#include <util.h>
#include <audit/client/tagger.h>
#include <audit/client/local_disk_storage.h>
#include <audit/client/prf.h>
#include <audit/client/verification.h>

void Upload(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  audit::Tagger tagger{
      std::unique_ptr<audit::Storage>{new audit::LocalDiskStorage{"test"}},
      std::unique_ptr<audit::RandomNumberGenerator>{
          new audit::CryptoNumberGenerator},
      std::unique_ptr<audit::PRF>{new audit::HMACPRF}};

  info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("upload").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Upload)->GetFunction());
}

NODE_MODULE(upload, Init)
