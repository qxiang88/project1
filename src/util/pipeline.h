//
// Created by wangqixiang on 2017/11/15.
//

#ifndef MEDIA_MANAGE_SYSTEM_PIPELINE_H
#define MEDIA_MANAGE_SYSTEM_PIPELINE_H

#include <functional>
#include <vector>
#include <string>
#include <memory>

#include "corgi/base/status.h"
#include "corgi/synchronization/mutex.h"
#include "corgi/env/env.h"

namespace mms {

class Pipeline {
 public:
  typedef std::function<corgi::Status(const std::string& message, bool is_last_block, std::string* result)> Filter;

  Pipeline(const std::string& input_file_path,
           const std::string& output_file_path);
  virtual ~Pipeline() = default;

  corgi::Status Init();
  void AddFilter(const Filter& filter);
  corgi::Status RemoveFilter(const Filter& filter);
  corgi::Status Process();

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(Pipeline);

  std::string input_file_path_;
  std::string output_file_path_;
  std::unique_ptr<corgi::RandomAccessFile> input_file_;
  std::unique_ptr<corgi::WritableFile> output_file_;
  corgi::uint64 total_input_file_size_;

  std::vector<Filter> filters_;
  corgi::mutex mu_;
};

} // namespace mms

#endif //MEDIA_MANAGE_SYSTEM_PIPELINE_H
