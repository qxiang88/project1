//
// Created by wangqixiang on 2017/11/19.
//

#ifndef MEDIA_MANAGE_SYSTEM_ACL_H
#define MEDIA_MANAGE_SYSTEM_ACL_H

#include "corgi/base/status.h"
#include "database/database.h"
#include "database/db_key_generator.h"

namespace mms {

class ACL {
 public:
  ACL(Database* database, DBKeyGenerator* db_key_generator);

  corgi::Status GenerateMediaKeyToken(const std::string &uuid,
                                      std::string *result);
  corgi::Status CheckKeyMediaToken(const std::string &uuid,
                                   const std::string &token);

 private:
  Database *database_;
  DBKeyGenerator* db_key_generator_;
  CG_DISALLOW_COPY_AND_ASSIGN(ACL);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_ACL_H
