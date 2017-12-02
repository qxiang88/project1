//
// Created by wangqixiang on 2017/11/14.
//

#include "filesystem/filesystem.h"
#include "task/task.h"
#include "corgi/strings/substitute.h"
#include "transcode/transcoder.h"
#include "util/directory_sizer.h"
#include "task/notifier.h"

namespace mms {

Task::Task(const TaskMetadata &metadata,
           CommandBuilder* command_builder,
           std::shared_ptr<TaskStorage> task_storage)
    : metadata_(metadata),
      task_storage_(task_storage) {
  command_builder_.reset(command_builder);
}

Task::~Task() {}

corgi::Status Task::ChangeStateAndStore(TaskState state,
                                        TaskState* old) {
  corgi::mutex_lock l(mu_);
  TaskState o = metadata_.state();
  metadata_.SetState(state);
  corgi::Status s = task_storage_->StoreTaskMetadata(metadata_.uuid(),
                                                     metadata_.task_metapb());
  if (!s.ok()) {
    return s;
  }
  if (old != nullptr) {
    *old = o;
  }
  return corgi::Status::OK();
}

std::string Task::uuid() const {
  // uuid 为不变量不需要加锁
  return metadata_.uuid();
}

TaskState Task::state() const {
  corgi::mutex_lock l(mu_);
  return metadata_.state();
}

void Task::set_notify_url(const std::string &notify_url) {
  notify_url_ = notify_url;
}

bool Task::has_notify_url() const {
  return !notify_url_.empty();
}

corgi::Status Task::NotifyTaskState() {
  corgi::Status status;
  if (has_notify_url()) {
    std::unique_ptr<Notifier> notifier;
    status = Notifier::Create(notify_url_, &notifier);
    if (!status.ok()) {
      LOG(ERROR) << "Error to create notifier for : " << notify_url_
          << " error: " << status.ToString();
      return status;
    }
    status = notifier->NotifyTaskState(metadata_.uuid(),
                                       metadata_.state(),
                                       metadata_.target_size());
    if (!status.ok()) {
      LOG(ERROR) << "Error notify to: " << notify_url_
          << " error: " << status.ToString();
      return status;
    }
    return status;
  } else {
    return corgi::errors::InvalidArgument("notify_url_ not set");
  }
}

///////////////////// HLSTranscodeTask

HLSTranscodeTask::HLSTranscodeTask(const TaskMetadata &metadata,
                                   CommandBuilder* command_builder,
                                   std::shared_ptr<TaskStorage> task_storage,
                                   const HLSMetaPB& hls_meta_pb)
    : Task(metadata, command_builder, task_storage) {
  hls_meta_pb_.CopyFrom(hls_meta_pb);
}

void HLSTranscodeTask::OnComplete() {
  // 获取目标大小:
  DirectorySizer target_sizer(Filesystem::Default()->GetLocalMediaRoot(metadata_.uuid()),
                              "*.ts");
  uint64_t total_ts_size;
  corgi::Status s = target_sizer.ComputeSizeRecursely(&total_ts_size);
  if (!s.ok()) {
    LOG(WARNING) << "Warning: compute size of : " << Filesystem::Default()->GetLocalMediaRoot(metadata_.uuid())
                 << " error: " << s.ToString();
  } else {
    metadata_.SetTargetSize(total_ts_size);
  }

  CG_LOG_IF_ERROR(ChangeStateAndStore(kTranscodeAndEncryptDone));
  // Copy 本地工作目录的文件到GFS挂载点
  CopierParameter param;
  param.src = Filesystem::Default()->GetLocalMediaRoot(metadata_.uuid());
  param.dst = Filesystem::Default()->GetGFSMediaRoot(metadata_.uuid());

  CG_LOG_IF_ERROR(ChangeStateAndStore(kCopying));
  corgi::Status status = Filesystem::Default()->Copy(param);
  if (status.ok()) {
    CG_LOG_IF_ERROR(ChangeStateAndStore(kCopyDone));
    // 存储HLS元数据
    CG_LOG_IF_ERROR(task_storage_->StoreHLSMetadata(hls_meta_pb_.uuid(), &hls_meta_pb_));
    // 发通知给 Client
    // 如果发送失败, 则我们只是打日志.
    // 后续, Client 可以进行主动查询.
    //
    status = NotifyTaskState();
    LOG(INFO) << "Notified uuid=" << uuid()
              << " to url=" << notify_url_
              << " : " << status.ToString();
    // 清理本地文件
    status = Filesystem::Default()->Cleanup(param.src);
    LOG(INFO) << "cleanup: " << param.src << " : " << status.ToString();
  } else {
    LOG(ERROR) << "Copy: " << param.src << " to " << param.dst
               << " error: " << status.ToString();
    CG_LOG_IF_ERROR(ChangeStateAndStore(kCopyError));
  }
  delete this;
}

corgi::Status HLSTranscodeTask::OnTranscode(Transcoder *transcoder) {
  if (metadata_.target_format() != "hls") {
    return corgi::errors::Unimplemented("not supported target format", metadata_.target_format());
  }
  Command command;
  CG_RETURN_IF_ERROR(command_builder_->BuildLowDefinitionCommand(&command));
  LOG(INFO) << "Command: " << command.ToString();
  transcoder->SetTranscodingStatCB([](const std::string& stats_info) {
    LOG(INFO) << "stat: " << stats_info << std::flush;
  });

  corgi::Status status = transcoder->Run(command);
  LOG(INFO) << "hls ffmpeg status: " << status.ToString();
  {
    LOG(INFO) << "store: ";
    LOG(INFO) << hls_meta_pb_.DebugString();
  }
  return status;
}

} // namespace mms
