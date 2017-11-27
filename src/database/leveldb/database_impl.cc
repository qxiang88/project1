//
// Created by wangqixiang on 2017/11/16.
//

#include "database/leveldb/database_impl.h"
#include "database/leveldb/db_key_generator_impl.h"
#include "corgi/base/errors.h"

#include "leveldb/write_batch.h"

namespace mms {

// 注册
namespace {
class LevelDBDatabaseRegistrar {
 public:
  LevelDBDatabaseRegistrar() {
    DatabaseFactory::Register("leveldb", new LevelDBDatabaseFactory());
  }
};
static LevelDBDatabaseRegistrar registrar;
} // namespace

namespace {

corgi::Status FromLevelDBStatus(const leveldb::Status &status,
                                const corgi::Slice& key="") {
  if (status.ok()) {
    return corgi::Status::OK();
  }
  if (status.IsCorruption()) {
    return corgi::errors::Internal(status.ToString());
  }
  if (status.IsIOError()) {
    return corgi::errors::Unavailable(status.ToString());
  }
  if (status.IsNotFound()) {
    return corgi::errors::NotFound("key=",key);
  }
  // TODO(xiang88):
  // 判断 LevelDB 版本号做兼容.
  #if 0
  if (status.IsNotSupportedError()) {
    return corgi::errors::Unimplemented(status.ToString());
  }
  if (status.IsInvalidArgument()) {
    return corgi::errors::InvalidArgument(status.ToString());
  }
  #endif
  return corgi::errors::Unknown("Unknown leveldb error");
}

} // namespace

LevelDBDatabase::LevelDBDatabase() {}

LevelDBDatabase::~LevelDBDatabase() {}

corgi::Status LevelDBDatabase::Open(const corgi::Slice &db_name) {
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, db_name.ToString(), &db);
  if (!status.ok()) {
    return FromLevelDBStatus(status);
  }
  db_.reset(db);
  return FromLevelDBStatus(status);
}

corgi::Status LevelDBDatabase::Get(const corgi::Slice &key,
                                  std::string *value) {
  leveldb::Status status = db_->Get(leveldb::ReadOptions(),
                                    key.ToString(),
                                    value);
  return FromLevelDBStatus(status, key);
}

corgi::Status LevelDBDatabase::Put(const corgi::Slice &key,
                                  const corgi::Slice &value) {
  leveldb::Status status = db_->Put(leveldb::WriteOptions(),
                                    key.ToString(), value.ToString());
  return FromLevelDBStatus(status, key);
}

corgi::Status LevelDBDatabase::Update(const corgi::Slice &key,
                                     const corgi::Slice &value) {
  std::string value2;
  leveldb::Status s = db_->Get(leveldb::ReadOptions(), key.ToString(), &value2);
  if (s.ok()) {
    leveldb::WriteBatch batch;
    batch.Delete(key.ToString());
    batch.Put(key.ToString(), value.ToString());
    s = db_->Write(leveldb::WriteOptions(), &batch);
  }
  return FromLevelDBStatus(s,key);
}

corgi::Status LevelDBDatabase::UpdateOrPut(const corgi::Slice &key,
                                           const corgi::Slice &value) {
  std::string v;
  corgi::Status s = Get(key, &v);
  if (s.ok()) {
    return Update(key, value);
  }
  if (corgi::errors::IsNotFound(s)) {
    return Put(key, value);
  }
  return s;
}

corgi::Status LevelDBDatabase::Delete(const corgi::Slice &key) {
  leveldb::Status s = db_->Delete(leveldb::WriteOptions(), key.ToString());
  return FromLevelDBStatus(s, key);
}

corgi::Status LevelDBDatabase::Close() {
  // TODO(xiang88):
  // 有点丑
  db_.reset(nullptr);
  return corgi::Status::OK();
}

/////////////////////////////////////

LevelDBDatabaseFactory::LevelDBDatabaseFactory() {}
LevelDBDatabaseFactory::~LevelDBDatabaseFactory() {}

corgi::Status LevelDBDatabaseFactory::NewDatabase(std::unique_ptr<Database> *result) {
  result->reset(new LevelDBDatabase);
  return corgi::Status::OK();
}

corgi::Status LevelDBDatabaseFactory::NewDBKeyGenerator(std::unique_ptr<DBKeyGenerator> *result) {
  result->reset(new LevelDBKeyGenerator);
  return corgi::Status::OK();
}

} // namespace mms
