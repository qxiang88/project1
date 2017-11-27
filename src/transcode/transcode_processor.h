//
// Created by wangqixiang on 2017/11/15.
//

#ifndef MEDIA_MANAGE_SYSTEM_TRANSCODE_PROCESSOR_H
#define MEDIA_MANAGE_SYSTEM_TRANSCODE_PROCESSOR_H

#include <vector>
#include <memory>
#include <thread>

#include "corgi/base/status.h"
#include "blockingconcurrentqueue.h"
#include "task/task.h"
#include "transcode/transcoder.h"
#include "corgi/synchronization/mutex.h"
#include "corgi/synchronization/notification.h"

namespace mms {

struct ProcessorOptions {
  // 队列最大长度
  corgi::int64 max_queue_size = 1000 * 10;
  // 处理器名字
  std::string name;
  // 等待任务的超时.
  // 最超时后, Processor 会检查是否线程(TranscodeManager)
  // 通知它退出, 这样可以优雅的让整个系统安全退出. 如果当前有任务
  // 正在处理, 那么我们将等到该处理结束再退出.
  // Default: 1 second
  std::chrono::milliseconds wait_task_timeout;

  ProcessorOptions() {}

  static ProcessorOptions Default() {
    ProcessorOptions r;
    r.name = "defaultProcessor";
    r.wait_task_timeout = std::chrono::milliseconds(1000);
    return r;
  }
};

// TranscodeProcess 表示一个转码处理器.
// 它包含一个生产者/消费者队列以及一个 Transcoder.
// Transcoder 是队列的消费者, 它负责从队列中获取 Task, 然后处理Task
// 的转码加密的流程. Transcoder 由一个独立的线程运行.
class TranscodeProcessor {
 public:
  static corgi::Status Create(const ProcessorOptions& options,
                              std::unique_ptr<TranscodeProcessor>* result);

  virtual ~TranscodeProcessor();

  // 提交任务到转码队列.
  // 最终由Transcoder 负责处理.
  corgi::Status Submit(Task *task);

  // 停止Processor
  void Stop();

 private:
  friend void ProcessorRunLoop(TranscodeProcessor* processor);

  ProcessorOptions options_;
  corgi::Status Init();
  TranscodeProcessor(const ProcessorOptions& options);

  moodycamel::BlockingConcurrentQueue<Task*> task_queue_;
  std::unique_ptr<Transcoder> transcoder_;
  std::unique_ptr<std::thread> worker_;
  corgi::mutex mu_;
  corgi::Notification notification_;

  CG_DISALLOW_COPY_AND_ASSIGN(TranscodeProcessor);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TRANSCODE_PROCESSOR_H
