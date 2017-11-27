//
// Created by wangqixiang on 2017/11/16.
//

#ifndef MEDIA_MANAGE_SYSTEM_STORAGE_IMPL_H
#define MEDIA_MANAGE_SYSTEM_STORAGE_IMPL_H

#include <memory>

#include "database/database.h"
#include "leveldb/db.h"

namespace mms {

class LevelDBDatabase : public Database {
 public:
  LevelDBDatabase();
  ~LevelDBDatabase();

  virtual corgi::Status Open(const corgi::Slice& db_name) override;
  virtual corgi::Status Close() override;
  virtual corgi::Status Get(const corgi::Slice& key, std::string* value) override;
  virtual corgi::Status Put(const corgi::Slice& key, const corgi::Slice& value) override;
  virtual corgi::Status Update(const corgi::Slice& key, const corgi::Slice& value) override;
  virtual corgi::Status UpdateOrPut(const corgi::Slice& key, const corgi::Slice& value) override;

  virtual corgi::Status Delete(const corgi::Slice& key) override;

 private:
  std::unique_ptr<leveldb::DB> db_;
  CG_DISALLOW_COPY_AND_ASSIGN(LevelDBDatabase);
};

class LevelDBDatabaseFactory : public DatabaseFactory {
 public:
  LevelDBDatabaseFactory();
  virtual ~LevelDBDatabaseFactory();

  virtual corgi::Status NewDatabase(std::unique_ptr<Database> *result) override;
  virtual corgi::Status NewDBKeyGenerator(std::unique_ptr<DBKeyGenerator>* result) override;
  virtual bool AcceptsOptions(const std::string& db_engine) override {
    return db_engine == "leveldb";
  }

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(LevelDBDatabaseFactory);
};

} // namespace mms

#endif //MEDIA_MANAGE_SYSTEM_STORAGE_IMPL_H
