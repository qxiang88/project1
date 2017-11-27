//
// Created by wangqixiang on 2017/11/20.
//

#include "task/task_storage.h"
#include "corgi/gtl/stl_util.h"
#include "corgi/base/errors.h"

namespace mms {

TaskStorage::TaskStorage(DBKeyGenerator *db_key_generator,
                         Database *database)
    : db_key_generator_(db_key_generator),
      database_(database) {}


namespace {

bool SerializePB(const google::protobuf::Message* message,
                 std::string* result) {
  std::string r;
  bool b = message->SerializeToString(&r);
  if (!b) {
    return false;
  }
  corgi::gtl::STLStringResizeUninitialized(result, r.size());
  result->swap(r);
  return true;
}

} // namespace

corgi::Status TaskStorage::StorePB(const std::string &key,
                                   const google::protobuf::Message *message) {
  std::string data;
  bool b = SerializePB(message, &data);
  if (!b) {
    return corgi::errors::Internal("Serialize PB error key= ", key);
  }
  return database_->UpdateOrPut(key, data);
}

corgi::Status TaskStorage::RetrievePB(const std::string &key,
                                      google::protobuf::Message *message) {
  std::string data;
  CG_RETURN_IF_ERROR(database_->Get(key, &data));
  bool b = message->ParseFromString(data);
  if (!b) {
    return corgi::errors::Internal("Deserialize PB error key= ", key);
  }
  return corgi::Status::OK();
}

//////////////////

corgi::Status TaskStorage::StoreTaskMetadata(const std::string &uuid,
                                             const TaskMetaPB *meta) {
  std::string key = db_key_generator_->CreateTaskKey(uuid);
  return StorePB(key, meta);
}

corgi::Status TaskStorage::RetrieveTaskMetadata(const std::string &uuid,
                                                TaskMetaPB *result) {
  std::string key = db_key_generator_->CreateTaskKey(uuid);
  return RetrievePB(key, result);
}

corgi::Status TaskStorage::StoreHLSMetadata(const std::string &uuid,
                                            const HLSMetaPB *meta) {
  std::string key = db_key_generator_->CreateHLSKey(uuid);
  return StorePB(key, meta);
}

corgi::Status TaskStorage::RetrieveHLSMetadata(const std::string &uuid,
                                               HLSMetaPB *result) {
  std::string key = db_key_generator_->CreateHLSKey(uuid);
  return RetrievePB(key, result);
}

} // namespace mms
