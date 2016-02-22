#pragma once

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

#include "audit/common.h"

using json = nlohmann::json;

namespace audit {

class Settings {
 public:
  Settings() {
    std::ifstream f{file_};
    settings_ = json::parse(f);
  }

  ~Settings() {
    if (changed_) {
      std::ofstream f{file_};
      f << settings_.dump();
    }
  }

  template <typename T>
  T Get(const std::string& key) {
    return settings_.at(key).get<T>();
  }

  template <typename T>
  void Set(const std::string& key, T val) {
    settings_[key] = val;
    changed_ = true;
  }

  bool Contains(const std::string& key) {
    return settings_.find(key) != settings_.end();
  }

 private:
  const std::string file_{application_dir + "settings.json"};

  bool changed_ = false;

  json settings_;
};
}
