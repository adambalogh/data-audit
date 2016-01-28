#pragma once

namespace audit {

class ProgressBar {
 public:
  ProgressBar(int max) : max_(max) {}

  void Progress(int value) {
    auto step = static_cast<double>(max_) / static_cast<double>(100);
    current_value_ += value;
    auto advance = static_cast<int>(static_cast<double>(current_value_) / step);
    percentage_ += advance;
    current_value_ -= advance * step;
  }

  int Percentage() const { return percentage_; }
  bool Done() const { return percentage_ == 100; }

 private:
  const int max_;

  int current_value_{0};

  int percentage_{0};
};
}
