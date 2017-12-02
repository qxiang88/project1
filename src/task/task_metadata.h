//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_TASK_METADATA_H
#define MEDIA_MANAGE_SYSTEM_TASK_METADATA_H

#include "proto/protocol.pb.h"

namespace mms {

enum TaskState {
  kNew = 0,
  kCopyDone = 1, // 不能调整此枚举值!!!
                 // 我们与Client端 约定返回 1 表示成功.
                 //
  kQueuedTranscodeAndEncrypt,
  kQueuedError,
  kTranscodingAndEncrypting,
  kTranscodingAndEncryptingError,
  kTranscodeAndEncryptDone,
  kCopying,
  kCopyError,
  kUnknown
};
std::string TaskStateToString(TaskState state);
// TaskState StringToTaskState(const std::string& str);

class TaskMetadata {
  friend class Task;
 public:
  TaskMetadata();
  TaskMetadata(const std::string& uuid);
  TaskMetadata(const std::string& uuid,
               TaskState state);

  void SetUUID(const std::string& uuid);
  void SetState(TaskState state);
  void SetTargetFormat(const std::string& format);
  void SetMediaSourcePath(const std::string& path);
  void SetTargetSize(uint64_t target_size);

  std::string uuid() const;
  TaskState state() const;
  std::string state_as_string() const;
  std::string target_format() const;
  std::string media_source_path() const;
  uint64_t target_size() const;

  std::string ToString() const;
  bool FromString(const std::string* str);

  const TaskMetaPB* task_metapb() const {
    return &meta_pb_;
  }

 private:
  TaskMetaPB meta_pb_;
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TASK_METADATA_H
