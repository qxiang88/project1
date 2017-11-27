//
// Created by wangqixiang on 2017/11/20.
//

#ifndef MEDIA_MANAGE_SYSTEM_TASK_STORAGE_H
#define MEDIA_MANAGE_SYSTEM_TASK_STORAGE_H

#include "database/database.h"
#include "proto/protocol.pb.h"

namespace mms {

// TODO(xiang88):
//  增加 TaskCache

class TaskStorage {
 public:
  TaskStorage(DBKeyGenerator* db_key_generator,
              Database* database);

  corgi::Status StoreTaskMetadata(const std::string& uuid,
                                  const TaskMetaPB* meta);
  corgi::Status RetrieveTaskMetadata(const std::string& uuid,
                                     TaskMetaPB* result);

  corgi::Status StoreHLSMetadata(const std::string& uuid,
                                 const HLSMetaPB* meta);
  corgi::Status RetrieveHLSMetadata(const std::string& uuid,
                                    HLSMetaPB* result);

 private:
  DBKeyGenerator* db_key_generator_;
  Database* database_;

  corgi::Status StorePB(const std::string& key,
                        const google::protobuf::Message* message);
  corgi::Status RetrievePB(const std::string& key,
                           google::protobuf::Message* message);

  CG_DISALLOW_COPY_AND_ASSIGN(TaskStorage);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TASK_STORAGE_H
