//
// Created by wangqixiang on 2017/11/15.
//

#include "transcode/transcode_processor.h"

namespace mms {

// friend
void ProcessorRunLoop(TranscodeProcessor *processor);

// static
corgi::Status TranscodeProcessor::Create(const ProcessorOptions &options,
                                         std::unique_ptr<TranscodeProcessor> *result) {
  TranscodeProcessor *r = new TranscodeProcessor(options);
  corgi::Status status = r->Init();
  if (!status.ok()) {
    delete r;
    return status;
  }
  result->reset(r);
  return status;
}

TranscodeProcessor::~TranscodeProcessor() {
  Stop();
  if (worker_->joinable()) {
    worker_->join();
  }
}

TranscodeProcessor::TranscodeProcessor(const ProcessorOptions &options)
    : options_(options),
      transcoder_(new Transcoder) {
}

corgi::Status TranscodeProcessor::Submit(Task *task) {
  corgi::mutex_lock l(mu_);

  size_t sz = task_queue_.size_approx();
  if (sz++ >= options_.max_queue_size) {
    return corgi::errors::OutOfRange("task queue size out of range: ", sz);
  }
  task_queue_.enqueue(task);
  return corgi::Status::OK();
}

corgi::Status TranscodeProcessor::Init() {
  worker_.reset(new std::thread(ProcessorRunLoop, this));
  return corgi::Status::OK();
}

void TranscodeProcessor::Stop() {
  corgi::mutex_lock l(mu_);
  if (!notification_.HasBeenNotified()) {
    notification_.Notify();
  }
}

// friend
// worker thread run loop
void ProcessorRunLoop(TranscodeProcessor *processor) {
  while (true) {
    Task *task;
    //LOG(INFO) << processor->options_.name << " wait task";
    bool got = processor->task_queue_.wait_dequeue_timed(task,
                                                         processor->options_.wait_task_timeout);
    if (got) {
      //LOG(INFO) << processor->options_.name << " get task: " << task->uuid();
      processor->transcoder_->Reset();

      // 注意:
      // 我们这在这里修改关于 Task 的转码加密的State.
      //
      task->OnProcessing();
      corgi::Status status = processor->transcoder_->Transcode(task);
      if (!status.ok()) {
        task->OnError("Transcoding and Encrypting error");
      } else {
        task->OnComplete();
      }
    }
    if (processor->notification_.HasBeenNotified()) {
      //LOG(INFO) << processor->options_.name << " existed";
      return;
    }
  }
}

} // namespace mms