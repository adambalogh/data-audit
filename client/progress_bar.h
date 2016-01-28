#pragma once

namespace audit {

class ProgressBar {
 public:
  ProgressBar(int max) : max_(max) {}

  void Progress(int value) {
    current_value_ += value;
    percentage_ =
        (static_cast<double>(current_value_) / static_cast<double>(max_)) * 100;
    if (percentage_ > 100) percentage_ = 100;
  }

  int Percentage() const { return percentage_; }

  bool Done() const { return percentage_ == 100; }

 private:
  // When the progress bar reaches this value, it's considered done
  const int max_;

  // This value measures the current progress relative to max
  int current_value_{0};

  int percentage_{0};
};
}
