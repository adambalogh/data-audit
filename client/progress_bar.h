#pragma once

#include <functional>

namespace audit {

class ProgressBar {
 public:
  typedef std::function<void(int)> CallbackType;

  ProgressBar(size_t max, CallbackType callback = [](int) {})
      : max_(max), callback_(callback) {}

  void Progress(size_t value) {
    current_value_ += value;

    size_t new_percentage = (current_value_ * 100) / max_;
    if (new_percentage > 100) new_percentage = 100;
    if (new_percentage > percentage_) {
      callback_(new_percentage);
    }
    percentage_ = new_percentage;
  }

  size_t Percentage() const { return percentage_; }

  bool Done() const { return percentage_ == 100; }

 private:
  // When the progress bar reaches this value, it's considered done
  const size_t max_;

  // This value measures the current progress relative to max
  size_t current_value_{0};

  size_t percentage_{0};

  // Should be called whenever the percentage increases
  CallbackType callback_;
};
}
