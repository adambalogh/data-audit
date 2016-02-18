#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "nan.h"
#include "audit/common.h"
#include "audit/files/file_list_source.h"

using v8::Array;
using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

using Nan::Callback;
using Nan::MakeCallback;
using Nan::GetCurrentContext;
using Nan::New;
using Nan::FunctionCallbackInfo;
using json = nlohmann::json;

using namespace audit::file_browser;

// This is where the name of the recent files are saved
const std::string recent_files_file =
    audit::application_dir + "recent_files.json";

// The number of recent files we keep
const int MAX_RECENT_FILES = 6;

void WriteRecentFiles(const std::vector<File>&);

class RecentFileListSource : public FileListSource {
 public:
  std::vector<File> GetFiles() {
    std::ifstream f{recent_files_file};
    // If file has not been created already, create it with empty list
    if (!f) {
      WriteRecentFiles({});
      return {};
    }
    auto files_json = json::parse(f);
    std::vector<File> files;
    for (auto file : files_json["files"]) {
      files.push_back(
          File{file["name"].get<std::string>(), file["size"].get<size_t>()});
    }
    return files;
  }
};

// Adds 1 file to the recent files list, and saves it
void AddFile(const std::string& file_name, size_t size) {
  RecentFileListSource s;
  auto files = s.GetFiles();
  std::vector<File> new_files;
  new_files.push_back(File{file_name, size});
  for (auto i = 0u; i < files.size() && new_files.size() < MAX_RECENT_FILES;
       ++i) {
    new_files.push_back(files[i]);
  }
  WriteRecentFiles(new_files);
}

void WriteRecentFiles(const std::vector<File>& files) {
  std::vector<json> json_files;
  for (auto& file : files) {
    json_files.push_back({{"name", file.name}, {"size", file.size}});
  }

  json files_json;
  files_json["files"] = json_files;

  std::ofstream f{recent_files_file};
  f << files_json.dump();
}

void GetRecentFiles(const FunctionCallbackInfo<Value>& info) {
  RecentFileListSource s;
  auto files = s.GetFiles();

  Local<Array> output = New<Array>(files.size());
  for (size_t i = 0; i < files.size(); ++i) {
    Local<Array> file = New<Array>(2);
    file->Set(0, New<String>(files[i].name).ToLocalChecked());
    file->Set(1, New<String>(files[i].Size()).ToLocalChecked());

    output->Set(i, file);
  }

  auto cb = info[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = {output};
  MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}
