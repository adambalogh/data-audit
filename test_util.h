#pragma once

#include <vector>

#include "audit/common.h"
#include "audit/client/prf.h"
#include "audit/util.h"

using namespace audit;

BN_ptr BN_new_ptr(unsigned int i) {
  BN_ptr num{BN_new(), ::BN_free};
  BN_set_word(num.get(), i);
  return std::move(num);
}

bool operator==(const long &a, const BN_ptr &b) {
  BN_ptr other{BN_new(), ::BN_free};
  BN_set_word(other.get(), a);
  return BN_cmp(b.get(), other.get()) == 0;
}

class DummyPRF : public PRF {
 public:
  DummyPRF() : PRF(0, 1) {}
  BN_ptr Encode(unsigned int i) { return BN_new_ptr(i); }
};

// Random number generator that returns 1 all the time
template <int n>
class ConstantNumberGenerator : public RandomNumberGenerator {
 public:
  BN_ptr GenerateNumber(const BIGNUM &) {
    BN_ptr num{BN_new(), ::BN_free};
    BN_set_word(num.get(), n);
    return std::move(num);
  }
};

// Random number generator that returns the numbers from the given vector, in
// sequence
class DummyNumberGenerator : public RandomNumberGenerator {
 public:
  DummyNumberGenerator(std::vector<int> nums) : nums_(nums) {}
  BN_ptr GenerateNumber(const BIGNUM &) override {
    BN_ptr num{BN_new(), ::BN_free};
    BN_set_word(num.get(), nums_[index++]);
    return std::move(num);
  }

 private:
  int index{0};
  std::vector<int> nums_;
};
