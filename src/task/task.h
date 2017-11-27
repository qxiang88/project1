//
// Created by wangqixiang on 2017/11/14.
//

#ifndef MEDIA_MANAGE_SYSTEM_TASK_H
#define MEDIA_MANAGE_SYSTEM_TASK_H

#include "corgi/base/status.h"
#include "corgi/base/errors.h"

#include "corgi/synchronization/mutex.h"

#include "task/task_metadata.h"
#include "task/command_builder.h"
#include "task/task_storage.h"

namespace mms {

class Transcoder;

class Task {
 public:
  Task(const TaskMetadata &metadata,
       CommandBuilder *command_builder,
       std::shared_ptr<TaskStorage> task_storage);

  virtual ~Task();

  corgi::Status Init() {
    return ChangeStateAndStore(kNew);
  }

  corgi::Status ChangeStateAndStore(TaskState state, TaskState *old = nullptr);

  std::string uuid() const;

  TaskState state() const;

  void set_notify_url(const std::string &notify_url);
  bool has_notify_url() const;

  // Note(xiang88):
  // 在 On* 系列回调中不能做耗时的操作. 这是因为实际上, 它是由
  // transcode::TrasncoderProcessor 中的线程执行的. 而这
  // 些线程主要的任务是做计算(转码加密); 要不然, 会影响系统的
  // 吞吐量.
  //
  // TODO(xiang88):
  // 我们要把转码成功的视频拷贝到目标路径, 清理在Workspace 目录
  // 下生成的文件.
  // 如果 Client 注册了 NotifyURL, 那么在特定的事件发生时应该
  // 让它们有权利知道! 拷贝完成了才进行通知, 因为只有拷贝完成了视频
  // 才可以被使用.
  //
  // 要增加一个进度回调.
  // virtual void OnUpdateStats(Stats stats) {}

  virtual void OnQueued() {
    LOG(INFO) << "Task uuid: " << metadata_.uuid() << " On queued";
    CG_LOG_IF_ERROR(ChangeStateAndStore(kQueuedTranscodeAndEncrypt));
  }

  virtual void OnProcessing() {
    LOG(INFO) << "Task uuid: " << metadata_.uuid() << " On processing";
    CG_LOG_IF_ERROR(ChangeStateAndStore(kTranscodingAndEncrypting));
  }

  virtual void OnError(std::string msg) {
    LOG(INFO) << "Task uuid: " << metadata_.uuid() << " On error: " << msg;
    CG_LOG_IF_ERROR(ChangeStateAndStore(kTranscodingAndEncryptingError));
    delete this;
  }

  virtual void OnComplete() {
    LOG(INFO) << "Task uuid: " << metadata_.uuid() << " On complete";
    CG_LOG_IF_ERROR(ChangeStateAndStore(kTranscodeAndEncryptDone));
    delete this;
  }

  virtual corgi::Status OnTranscode(Transcoder *transcoder) {
    return corgi::errors::Unimplemented("OnTranscode");
  }

  virtual corgi::Status OnNDTranscode(Transcoder *transcoder) {
    return corgi::errors::Unimplemented("OnNDTranscode");
  }

  virtual corgi::Status OnHDTranscode(Transcoder *transcoder) {
    return corgi::errors::Unimplemented("OnHDTranscode");
  }

  virtual corgi::Status NotifyTaskState();

 protected:
  TaskMetadata metadata_;
  mutable corgi::mutex mu_;
  std::unique_ptr<CommandBuilder> command_builder_;
  std::shared_ptr<TaskStorage> task_storage_;
  std::string notify_url_;

  CG_DISALLOW_COPY_AND_ASSIGN(Task);
};

class HLSTranscodeTask : public Task {
 public:
  HLSTranscodeTask(const TaskMetadata &metadata,
                   CommandBuilder *command_builder,
                   std::shared_ptr<TaskStorage> task_storage,
                   const HLSMetaPB &hls_meta_pb);

  virtual void OnComplete() override;

  virtual corgi::Status OnTranscode(Transcoder *transcode) override;

 private:
  HLSMetaPB hls_meta_pb_;
};

// TODO(xiang88):
//
class MP4TranscodeTask : public Task {
 public:
  MP4TranscodeTask(const TaskMetadata &metadata,
                   CommandBuilder *command_builder,
                   std::shared_ptr<TaskStorage> task_storage)
      : Task(metadata, command_builder, task_storage) {}

 private:
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TASK_H
