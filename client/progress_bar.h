#pragma once

#include <functional>
#include <mutex>

namespace audit {

// ProgressBar is a simple abstraction for a visual progress bar.
//
// It is safe to call from multiple threads.
//
class ProgressBar {
 public:
  typedef std::function<void(int)> CallbackType;

  // Constructs a ProgressBar
  //
  // @param max: when we reach this value, the ProgressBar is considered to be
  //   done
  // @param callback: this function is called with the current percentage,
  //   whenever it increases
  //
  ProgressBar(size_t max, CallbackType callback = [](int) {})
      : max_(max), callback_(callback) {}

  void Progress(size_t value) {
    std::lock_guard<std::mutex> l{mu_};

    current_value_ += value;
    size_t new_percentage = (current_value_ * 100) / max_;
    if (new_percentage > 100) new_percentage = 100;
    if (new_percentage > percentage_) {
      callback_(new_percentage);
    }
    percentage_ = new_percentage;
  }

  size_t Percentage() {
    std::lock_guard<std::mutex> l{mu_};
    return percentage_;
  }

  // Returns true if the progress bar is done
  bool Done() {
    std::lock_guard<std::mutex> l{mu_};
    return percentage_ == 100;
  }

 private:
  // When the progress bar reaches this value, it's considered done
  const size_t max_;

  // Guards current_value, percentage and callback
  std::mutex mu_;

  // This value measures the current progress relative to max
  size_t current_value_{0};

  size_t percentage_{0};

  // Should be called whenever the percentage increases
  CallbackType callback_;
};
}
