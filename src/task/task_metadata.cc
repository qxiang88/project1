//
// Created by wangqixiang on 2017/11/18.
//

#include "task/task_metadata.h"
#include "util/time_util.h"
#include "corgi/gtl/map_util.h"

#include <unordered_map>

namespace mms {

std::string TaskStateToString(TaskState state) {
  switch (state) {
    case kNew:
      return "NEW";
    case kQueuedTranscodeAndEncrypt:
      return "QUEUED";
    case kQueuedError:
      return "QUEUED_ERROR";
    case kTranscodingAndEncrypting:
      return "TRANS_ENC_NOW";
    case kTranscodingAndEncryptingError:
      return "TRANS_ENC_ERROR";
    case kTranscodeAndEncryptDone:
      return "TRANS_ENC_DONE";
    case kCopying:
      return "COPYING";
    case kCopyDone:
      return "COPY_DONE";
    case kCopyError:
      return "COPY_ERROR";
    default:
      return "UNKNOWN";
  }
}

TaskMetadata::TaskMetadata() {
  SetState(kNew);
}

TaskMetadata::TaskMetadata(const std::string& uuid) {
  SetState(kNew);
  meta_pb_.set_uuid(uuid);
}

TaskMetadata::TaskMetadata(const std::string &uuid, TaskState state) {
  SetState(kNew);
  meta_pb_.set_uuid(uuid);
  meta_pb_.set_state_int(state);
  meta_pb_.set_state_str(TaskStateToString(state));
}

void TaskMetadata::SetUUID(const std::string& uuid) {
  meta_pb_.set_uuid(uuid);
}

void TaskMetadata::SetMediaSourcePath(const std::string &path) {
  meta_pb_.set_media_source_path(path);
}

void TaskMetadata::SetState(TaskState state) {
  meta_pb_.set_state_int(state);
  meta_pb_.set_state_str(TaskStateToString(state));
  if (state == kNew) {
    meta_pb_.set_create_at(TimeUtil::FormatNow());
  } else {
    meta_pb_.set_last_update_at(TimeUtil::FormatNow());
  }
}

void TaskMetadata::SetTargetFormat(const std::string &format) {
  meta_pb_.set_target_format(format);
}

std::string TaskMetadata::uuid() const {
  return meta_pb_.uuid();
}

std::string TaskMetadata::state_as_string() const {
  return meta_pb_.state_str();
}

std::string TaskMetadata::media_source_path() const {
  return meta_pb_.media_source_path();
}

TaskState TaskMetadata::state() const {
  return static_cast<TaskState>(meta_pb_.state_int());
}

std::string TaskMetadata::target_format() const {
  return meta_pb_.target_format();
}

// Serialize/De-serialize
std::string TaskMetadata::ToString() const {
  std::string r;
  assert(meta_pb_.SerializeToString(&r));
  return r;
}

bool TaskMetadata::FromString(const std::string *str) {
  return meta_pb_.ParseFromString(*str);
}


} // namespace mms
