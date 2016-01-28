#pragma once

namespace audit {

class ProgressBar {
 public:
  ProgressBar(size_t max) : max_(max) {}

  void Progress(size_t value) {
    current_value_ += value;
    percentage_ =
        (static_cast<double>(current_value_) / static_cast<double>(max_)) * 100;
    if (percentage_ > 100) percentage_ = 100;
  }

  size_t Percentage() const { return percentage_; }

  bool Done() const { return percentage_ == 100; }

 private:
  // When the progress bar reaches this value, it's considered done
  const size_t max_;

  // This value measures the current progress relative to max
  size_t current_value_{0};

  size_t percentage_{0};
};
}
