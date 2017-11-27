//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_DB_KEY_H
#define MEDIA_MANAGE_SYSTEM_DB_KEY_H

#include <string>
#include <memory>

#include "corgi/base/status.h"

namespace mms {

class DBKeyGenerator {
 public:
  DBKeyGenerator();
  virtual ~DBKeyGenerator();

  virtual std::string CreateTaskKey(const std::string &guid) = 0;
  virtual std::string CreateHLSKey(const std::string &guid) = 0;
  virtual std::string CreateTokenKey(const std::string &guid) = 0;

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(DBKeyGenerator);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_DB_KEY_H
