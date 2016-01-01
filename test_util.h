#pragma once

#include <vector>
#include <sstream>

#include "audit/common.h"
#include "audit/client/prf.h"
#include "audit/util.h"
#include "audit/server/fetcher.h"
#include "audit/client/upload/file.h"
#include "audit/proto/cpor.pb.h"

using namespace audit;

BN_ptr BN_new_ptr(unsigned int i) {
  BN_ptr num{BN_new(), ::BN_free};
  BN_set_word(num.get(), i);
  return std::move(num);
}

std::vector<BN_ptr> make_BN_vector(const std::vector<unsigned int> &nums) {
  std::vector<BN_ptr> result;
  for (auto num : nums) {
    result.push_back(BN_new_ptr(num));
  }
  return result;
}

bool operator==(const BN_ptr &a, const BN_ptr &b) {
  return BN_cmp(b.get(), b.get()) == 0;
}

bool operator==(const long &a, const BN_ptr &b) {
  BN_ptr second{BN_new(), ::BN_free};
  BN_set_word(second.get(), a);
  return b == second;
}

void operator*=(BN_ptr &a, const BN_ptr &b) {
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};
  BN_mul(a.get(), a.get(), b.get(), ctx.get());
}

void operator*=(BN_ptr &a, unsigned int b) {
  auto second = BN_new_ptr(b);
  a *= second;
}

void operator+=(BN_ptr &a, const BN_ptr &b) {
  BN_add(a.get(), a.get(), b.get());
}

void operator+=(BN_ptr &a, unsigned int b) {
  auto second = BN_new_ptr(b);
  a += second;
}

void operator%=(BN_ptr &a, const BN_ptr &b) {
  BN_CTX_ptr ctx{BN_CTX_new(), ::BN_CTX_free};
  BN_mod(a.get(), a.get(), b.get(), ctx.get());
}

void operator%=(BN_ptr &a, unsigned int b) {
  auto second = BN_new_ptr(b);
  a %= second;
}

class DummyPRF : public PRF {
 public:
  DummyPRF() {}
  BN_ptr Encode(unsigned int i) const override { return BN_new_ptr(i); }
  std::string Key() const override { return ""; }
};

template <int n>
class ConstantPRF : public PRF {
 public:
  ConstantPRF() {}
  BN_ptr Encode(unsigned int i) const override { return BN_new_ptr(n); }
  std::string Key() const override { return ""; }
};

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

class MemoryFetcher : public Fetcher {
 public:
  MemoryFetcher(const upload::FileContext &context,
                std::vector<proto::BlockTag> &tags, std::stringstream &s)
      : context_(context), tags_(tags), s_(s) {}
  std::basic_istream<char, std::char_traits<char>> &FetchBlock(
      unsigned long index) {
    std::string block{
        s_.str().data() + context_.parameters().block_size() * index,
        std::min(
            context_.parameters().block_size(),
            s_.str().size() - (context_.parameters().block_size() * index))};
    stream.str(block);
    return stream;
  }

  proto::BlockTag FetchBlockTag(unsigned long index) { return tags_.at(index); }

 private:
  const upload::FileContext &context_;
  std::stringstream stream;
  std::vector<proto::BlockTag> &tags_;
  std::stringstream &s_;
};
