#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "audit/common.h"

using json = nlohmann::json;

// This module allows us to keep track of the most recently uploaded files

// This is where the name of the recent files are saved
const std::string recent_files_file =
    audit::application_dir + "recent_files.json";

// The number of recent files we keep
const int MAX_RECENT_FILES = 10;

void WriteRecentFiles(const std::vector<std::string>&);

// Returns the most recently uploaded files
std::vector<std::string> ReadRecentFiles() {
  std::ifstream f{recent_files_file};
  // If file has not been created already, create it with empty list
  if (!f) {
    WriteRecentFiles({});
    return {};
  }
  auto files_json = json::parse(f);
  std::vector<std::string> files;
  for (auto file : files_json["files"]) {
    files.push_back(file.get<std::string>());
  }
  return files;
}

// Adds 1 file to the recent files list, and saves it
void AddFile(const std::string& file_name) {
  auto files = ReadRecentFiles();
  files.insert(files.begin(), file_name);
  if (files.size() > MAX_RECENT_FILES) {
    files.pop_back();
  }
  WriteRecentFiles(files);
}

void WriteRecentFiles(const std::vector<std::string>& files) {
  std::ofstream f{recent_files_file};
  json files_json;
  files_json["files"] = files;
  f << files_json.dump();
}
