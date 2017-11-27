//
// Created by wangqixiang on 2017/11/15.
//

#include "util/pipeline.h"

namespace mms {

Pipeline::Pipeline(const std::string &input_file_path,
                   const std::string &output_file_path)
    : input_file_path_(input_file_path),
      output_file_path_(output_file_path),
      total_input_file_size_(0) {
}

corgi::Status Pipeline::Init() {
  CG_RETURN_IF_ERROR(corgi::Env::Default()->NewRandomAccessFile(input_file_path_, &input_file_));
  CG_RETURN_IF_ERROR(corgi::Env::Default()->NewWritableFile(output_file_path_, &output_file_));
  CG_RETURN_IF_ERROR(corgi::Env::Default()->GetFileSize(input_file_path_, &total_input_file_size_));
  return corgi::Status::OK();
}

void Pipeline::AddFilter(const Filter &filter) {
  corgi::mutex_lock l(mu_);
  // TODO(xiang88):
  #if 0
  auto r = std::find(filters_.begin(), filters_.end(), filter);
  if (r != filters_.end()) {
    return corgi::errors::AlreadyExists("filter already exist");
  }
  #endif
  filters_.push_back(filter);
  // return corgi::Status::OK();
}

corgi::Status Pipeline::RemoveFilter(const Filter &filter) {
  corgi::mutex_lock l(mu_);
  //TODO(xiang88):
  #if 0
  auto r = std::find(filters_.begin(), filters_.end(), filter);
  if (r != filters_.end()) {
    filters_.erase(r);
  }
  #endif
  return corgi::Status::OK();
}

corgi::Status Pipeline::Process() {
  char buf[1024];
  corgi::Slice input_buf;
  corgi::Status status;
  corgi::uint64 already_read_size = 0;

  while (true) {
    status = input_file_->Read(already_read_size, 1024, &input_buf, buf);
    if (!status.ok()) {
      if (status.code() != corgi::Code::OUT_OF_RANGE) {
        return status;
      }
    }

    already_read_size += input_buf.size();

    bool is_last_block = already_read_size >= total_input_file_size_;
    std::string result;
    for (auto& f : filters_) {
      CG_RETURN_IF_ERROR(f(input_buf.ToString(), is_last_block, &result));
    }
    CG_RETURN_IF_ERROR(output_file_->Append(result));
    if (is_last_block) {
      break;
    }
  }
  return corgi::Status::OK();
}

} // namespace mms
