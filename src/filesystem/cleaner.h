//
// Created by wangqixiang on 2017/11/21.
//

#ifndef MEDIA_MANAGE_SYSTEM_CLEANER_H
#define MEDIA_MANAGE_SYSTEM_CLEANER_H

#include <string>
#include "corgi/base/status.h"

namespace mms {

class Cleaner {
 public:
  virtual ~Cleaner() {}

  virtual corgi::Status Cleanup(const std::string& path) = 0;
};

class SimpleCleaner : public Cleaner {
 public:
  virtual ~SimpleCleaner() {}
  virtual corgi::Status Cleanup(const std::string& path) override;
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_CLEANER_H
