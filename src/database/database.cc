//
// Created by wangqixiang on 2017/11/14.
//

#include "database/database.h"

#include <unordered_map>

#include "corgi/base/errors.h"
#include "corgi/synchronization/mutex.h"


namespace mms {

namespace {

corgi::mutex* get_database_factory_lock() {
  static corgi::mutex database_factory_lock;
  return &database_factory_lock;
}

typedef std::unordered_map<std::string,
		                   DatabaseFactory*> DatabaseFactories;
DatabaseFactories* database_factories() {
  static DatabaseFactories* factories = new DatabaseFactories;
  return factories;
} 

} // namespace


Database::~Database() {}

DatabaseFactory::~DatabaseFactory() {}

// static
void DatabaseFactory::Register(const std::string& db_engine,
				               DatabaseFactory* factory) {
  corgi::mutex_lock l(*get_database_factory_lock());
  if (!database_factories()->insert({db_engine, factory}).second) {
    LOG(ERROR) << "Two database factories are being registered under "
			   << db_engine;
  }
}

// static
corgi::Status DatabaseFactory::GetFactory(const std::string& db_engine,
				                          DatabaseFactory** out_factory) {
  corgi::mutex_lock l(*get_database_factory_lock());
  for (const auto& database_factory : *database_factories()) {
    if (database_factory.second->AcceptsOptions(db_engine)) {
	  *out_factory = database_factory.second;
	  return corgi::Status::OK();
	}
  }
  return corgi::errors::NotFound(
      "No database factory registered for the given DbEngine: ",
	  db_engine);
}

corgi::Status NewDatabase(const std::string& db_engine,
				          std::unique_ptr<Database>* out_database) {
  DatabaseFactory* factory;
  CG_RETURN_IF_ERROR(DatabaseFactory::GetFactory(db_engine, &factory));
  return factory->NewDatabase(out_database);
}

corgi::Status NewDBKeyGenerator(const std::string& db_engine,
				                std::unique_ptr<DBKeyGenerator>* result) {
  DatabaseFactory* factory;
  CG_RETURN_IF_ERROR(DatabaseFactory::GetFactory(db_engine, &factory));
  return factory->NewDBKeyGenerator(result);
}

} // namespace mms
