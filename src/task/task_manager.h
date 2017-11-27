//
// Created by wangqixiang on 2017/11/16.
//

#ifndef MEDIA_MANAGE_SYSTEM_TASK_MANAGER_H
#define MEDIA_MANAGE_SYSTEM_TASK_MANAGER_H

#include "task/task.h"
#include "task/task_storage.h"

#include "database/database.h"
#include "filesystem/filesystem.h"
#include "acl/acl.h"
#include "config/conf.h"

#include "transcode/transcode_manager.h"

namespace mms {

// 整个系统的 Facade
class TaskManager {
 public:
  TaskManager(const std::string& conf_path);

  virtual ~TaskManager();

  corgi::Status Init();

  corgi::Status NewTask(const TaskMetadata& metadata,
                        std::unique_ptr<Task>* result);

  // 注意:
  // 当调用此方法时, task 的所有权已经为 TaskManager所用有.
  // TaskManager 会在特定的时机释放该 Task. 所以, Client
  // 不应该对 Task 做任何的释放操作.
  corgi::Status ScheduleTask(Task* task);

  // TODO(xiang88):
  // implement sooooon!
  // corgi::Status StoreTaskMeta(Task* task);
  corgi::Status RetrieveTaskMeta(const std::string& guid,
                                 TaskMetaPB* task_meta);
  corgi::Status RetrieveHLSMeta(const std::string& guid,
                                HLSMetaPB* hls_meta);
  corgi::Status CreateAccessKeyURI(const std::string& guid,
                                     std::string* result);
  corgi::Status RetrieveHLSKeyIfAllowed(const std::string& guid,
                                        const std::string& token,
                                        std::string* result);

 private:
  std::string conf_path_;
  Conf* conf_;
  Filesystem* filesystem_;

  std::unique_ptr<DBKeyGenerator> db_key_generator_;
  std::unique_ptr<Database> database_;
  std::unique_ptr<TranscodeManager> transcode_manager_;
  std::unique_ptr<ACL> acl_;

  // 每当创建一个 Task时, 我们会让它共享 task_storage_, 这样
  // 它们就可以根据需要调用 task_storage_ 更新数据, 它们更多
  // 调用的是 Store* 方法; 同时, TaskManager 也为它的Client
  // 提供 Retrieve* 方法. 由于 Task 在Submit以后就由别的线程
  // 在调用了, 我们是不是应该在 TaskStorage 方法里加锁呢????
  //
  std::shared_ptr<TaskStorage> task_storage_;

  CG_DISALLOW_COPY_AND_ASSIGN(TaskManager);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TASK_MANAGER_H
