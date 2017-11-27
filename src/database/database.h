//
// Created by wangqixiang on 2017/11/14.
//

#ifndef MEDIA_MANAGE_SYSTEM_STORAGE_H
#define MEDIA_MANAGE_SYSTEM_STORAGE_H

#include <memory>

#include "corgi/base/status.h"
#include "corgi/base/slice.h"
#include "database/db_key_generator.h"

namespace mms {

class Database {
 public:
  virtual ~Database();
  virtual corgi::Status Open(const corgi::Slice& db) = 0;
  virtual corgi::Status Get(const corgi::Slice& key, std::string* value) = 0;
  virtual corgi::Status Put(const corgi::Slice& key, const corgi::Slice& value) = 0;
  virtual corgi::Status Update(const corgi::Slice& key, const corgi::Slice& value) = 0;
  virtual corgi::Status UpdateOrPut(const corgi::Slice& key, const corgi::Slice& value) = 0;

  virtual corgi::Status Delete(const corgi::Slice& key) = 0;

  virtual corgi::Status Close() = 0;
};

class DatabaseFactory {
 public:
  virtual ~DatabaseFactory();
  virtual corgi::Status NewDatabase(std::unique_ptr<Database> *result) = 0;
  virtual corgi::Status NewDBKeyGenerator(std::unique_ptr<DBKeyGenerator>* result) = 0;

  virtual bool AcceptsOptions(const std::string& db_engine) = 0;

  static void Register(const std::string& db_engine, DatabaseFactory* factory);
  static corgi::Status GetFactory(const std::string& db_engine,
				                  DatabaseFactory** out_factory);
};

extern corgi::Status NewDatabase(const std::string& db_engine,
				                 std::unique_ptr<Database>* result);
extern corgi::Status NewDBKeyGenerator(const std::string& db_engine,
				                 std::unique_ptr<DBKeyGenerator>* result);

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_STORAGE_H
