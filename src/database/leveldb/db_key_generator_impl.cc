//
// Created by wangqixiang on 2017/11/20.
//

#include "database/leveldb/db_key_generator_impl.h"

namespace mms {

//////// LevelDB Implementation
const std::string kLevelDBTaskPrefix = "task:";
const std::string kLevelDBHLSPrefix = "hls:";
const std::string kLevelDBACLPrefix = "acl:";

std::string LevelDBKeyGenerator::CreateTaskKey(const std::string &guid) {
  return kLevelDBTaskPrefix + guid;
}

std::string LevelDBKeyGenerator::CreateHLSKey(const std::string &guid) {
  return kLevelDBHLSPrefix + guid;
}

std::string LevelDBKeyGenerator::CreateTokenKey(const std::string &guid) {
  return kLevelDBACLPrefix + guid;
}


} // namespace mms
