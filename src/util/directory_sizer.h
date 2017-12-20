//
// Created by wangqixiang on 2017/12/2.
//

#ifndef MEDIA_MANAGE_SYSTEM_DIRECTORY_SIZE_H
#define MEDIA_MANAGE_SYSTEM_DIRECTORY_SIZE_H

#include "corgi/base/macros.h"
#include "corgi/base/slice.h"
#include "corgi/base/status.h"

#include <string>

namespace mms {

class DirectorySizer {
 public:
  explicit DirectorySizer(const std::string& path);
  DirectorySizer(const std::string& path,
                corgi::Slice pattern);

  corgi::Status ComputeSize(uint64_t* result);
  corgi::Status ComputeSizeRecursively(uint64_t *result);

 private:
  corgi::Status DoComputeSize(bool recurse, uint64_t* result);

  std::string path_;
  std::string pattern_;
  CG_DISALLOW_COPY_AND_ASSIGN(DirectorySizer);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_DIRECTORY_SIZE_H
