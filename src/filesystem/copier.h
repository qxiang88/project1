//
// Created by wangqixiang on 2017/11/21.
//

#ifndef MEDIA_MANAGE_SYSTEM_COPIER_H
#define MEDIA_MANAGE_SYSTEM_COPIER_H

#include <string>
#include "corgi/base/status.h"

namespace mms {

struct CopierParameter {
  std::string src;
  std::string dst;
  bool error_if_exist{true};
};

class Copier {
 public:
  virtual ~Copier() {}
  virtual corgi::Status Copy(CopierParameter parameter) = 0;
};

class SimpleCopier : public Copier {
 public:
  virtual ~SimpleCopier() {}
  virtual corgi::Status Copy(CopierParameter parameter) override;
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_COPIER_H
