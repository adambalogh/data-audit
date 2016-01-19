#include <nan.h>

#include "audit/files/file_browser.h"
#include "audit/files/file_list_source.h"
#include "audit/files/local_file_list_source.h"

void GetFiles(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  audit::FileBrowser file_browser{
      std::unique_ptr<audit::FileListSource>{new audit::LocalFileListSource}};
  v8::Local<v8::Function> cb;
  std::vector<const std::string> files;

  if (info.Length() == 2) {
    v8::String::Utf8Value param1{info[0]->ToString()};
    std::string query{*param1};

    cb = info[1].As<v8::Function>();
    files = file_browser.Search(query);
  } else {
    cb = info[0].As<v8::Function>();
    files = file_browser.GetAllFiles();
  }

  v8::Local<v8::Array> output = Nan::New<v8::Array>(files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    output->Set(i, Nan::New(files[i]).ToLocalChecked());
  }

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = {output};
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("get_files").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetFiles)->GetFunction());
}

NODE_MODULE(file_browser, Init)
