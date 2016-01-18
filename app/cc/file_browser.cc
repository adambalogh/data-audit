#include <nan.h>

#include "audit/client/local_file_browser.h"

void ShowFiles(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  audit::LocalFileBrowser file_browser;
  auto files = file_browser.GetAllFiles();

  v8::Local<v8::Array> output = Nan::New<v8::Array>(files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    output->Set(i, Nan::New(files[i]).ToLocalChecked());
  }

  v8::Local<v8::Function> cb = info[0].As<v8::Function>();
  const unsigned argc = 1;

  v8::Local<v8::Value> argv[argc] = {output};
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("show_files").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(ShowFiles)->GetFunction());
}

NODE_MODULE(file_browser, Init)
