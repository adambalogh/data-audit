#include "audit/client/block_tagger.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <queue>
#include <thread>

#include "cryptopp/integer.h"
#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/util.h"
#include "audit/client/file_tag.h"
#include "audit/client/prf.h"
#include "audit/proto/cpor.pb.h"

namespace audit {

void BlockTagger::Worker::AddJob(JobArgs job) { jobs_.push(job); }

void BlockTagger::Worker::Loop() {
  while (jobs_.size()) {
    CalculateSigma(jobs_.front());
    jobs_.pop();
  }
}

void BlockTagger::Worker::CalculateSigma(JobArgs job) {
  BN_bin2bn(job.buffer, job.length, sector_.get());
  BN_mul(sector_.get(), job.alpha, sector_.get(), ctx_.get());
  BN_add(sigma_, sigma_, sector_.get());
  BN_clear(sector_.get());
}

BlockTagger::BlockTagger(const FileTag& file_tag, std::unique_ptr<PRF> prf)
    : file_tag_(file_tag), prf_(std::move(prf)) {
  buffer.resize(file_tag_.block_size());
  for (int i = 0; i < 10; ++i) {
    worker_sigmas_.push_back(BN_ptr{BN_new(), ::BN_free});
    workers_.push_back(Worker{worker_sigmas_.at(i).get()});
  }
  worker_threads_.resize(10);
  for (int i = 0; i < workers_.size(); ++i) {
    worker_threads_[i] = std::thread(&Worker::Loop, &workers_[i]);
  }
}

BlockTagger::~BlockTagger() {
  for (int i = 0; i < workers_.size(); ++i) {
    worker_threads_[i].join();
  }
}

void BlockTagger::ReadBlock() {
  file_tag_.file().read((char*)buffer.data(), file_tag_.block_size());
  end_ = file_tag_.file().gcount();
  if (end_ != file_tag_.block_size()) {
    file_read_ = true;
    assert(num_blocks_read_ + 1 == file_tag_.num_blocks());
  }
}

proto::BlockTag BlockTagger::GenerateTag() {
  ReadBlock();
  start_ = 0;

  BN_ptr sigma{BN_new(), ::BN_free};
  auto encoded_index = prf_->Encode(num_blocks_read_);
  BN_add(sigma.get(), sigma.get(), encoded_index.get());

  int worker_index = 0;
  // Go through each sector in block
  for (auto i = 0; i < file_tag_.num_sectors(); ++i) {
    workers_[i].AddJob(JobArgs{
        buffer.data() + start_,
        // TODO fix this hack
        (int)std::min(file_tag_.sector_size(), (unsigned long)end_ - start_),
        file_tag_.alphas().at(i).get()});
    start_ += file_tag_.sector_size();
    worker_index = (worker_index + 1) % workers_.size();
  }

  for (auto& s : worker_sigmas_) {
    BN_add(sigma.get(), sigma.get(), s.get());
  }
  // sigma = sigma % p
  BN_mod(sigma.get(), sigma.get(), file_tag_.p(), ctx.get());

  proto::BlockTag tag;
  tag.set_index(num_blocks_read_++);
  BignumToString(*sigma, tag.mutable_sigma());

  return tag;
}

proto::BlockTag BlockTagger::GetNext() { return GenerateTag(); }

bool BlockTagger::HasNext() const {
  return num_blocks_read_ < file_tag_.num_blocks();
}
}
