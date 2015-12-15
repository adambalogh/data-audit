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

void operator*=(BN_ptr &a, unsigned int b) {
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};
  auto second = BN_new_ptr(b);
  BN_mul(a.get(), a.get(), second.get(), ctx.get());
}

void operator+=(BN_ptr &a, unsigned int b) {
  auto second = BN_new_ptr(b);
  BN_add(a.get(), a.get(), second.get());
}

void operator%=(BN_ptr &a, unsigned int b) {
  auto second = BN_new_ptr(b);
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};
  BN_mod(a.get(), a.get(), second.get(), ctx.get());
}

class DummyPRF : public PRF {
 public:
  DummyPRF() : PRF("") {}
  BN_ptr Encode(unsigned int i) { return BN_new_ptr(i); }
};

template <int n>
class ConstantPRF : public PRF {
 public:
  ConstantPRF() : PRF("") {}
  BN_ptr Encode(unsigned int i) { return BN_new_ptr(n); }
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
