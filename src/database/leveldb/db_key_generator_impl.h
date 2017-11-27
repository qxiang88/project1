//
// Created by wangqixiang on 2017/11/20.
//

#ifndef MEDIA_MANAGE_SYSTEM_DB_KEY_GENERATOR_IMPL_H
#define MEDIA_MANAGE_SYSTEM_DB_KEY_GENERATOR_IMPL_H

#include "database/db_key_generator.h"

namespace mms {

class LevelDBKeyGenerator : public DBKeyGenerator {
 public:
  LevelDBKeyGenerator() {}
  virtual ~LevelDBKeyGenerator() override {}

  virtual std::string CreateTaskKey(const std::string &guid) override;
  virtual std::string CreateHLSKey(const std::string &guid) override;
  virtual std::string CreateTokenKey(const std::string &guid) override;

};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_DB_KEY_GENERATOR_IMPL_H
