//
// Created by wangqixiang on 2017/11/15.
//

#include "transcode/transcode_manager.h"
#include "corgi/base/errors.h"

namespace mms {

// static
corgi::Status TranscodeManager::Create(const TranscodeManagerOptions &options,
                                       std::unique_ptr<TranscodeManager> *result) {
  TranscodeManager *r = new TranscodeManager(options);
  corgi::Status status = r->Init();
  if (!status.ok()) {
    delete r;
    return status;
  }
  result->reset(r);
  return corgi::Status::OK();
}

TranscodeManager::TranscodeManager(const TranscodeManagerOptions &options)
    : options_(options) {}

TranscodeManager::~TranscodeManager() {
  StopAllProcessors();
}

void TranscodeManager::StopAllProcessors() {
  if (options_.enable_hd_task) processors_[kHighDefinition]->Stop();
  if (options_.enable_nd_task) processors_[kNormalDefinition]->Stop();
  if (options_.enable_ld_task) processors_[kLowDefinition]->Stop();
}

corgi::Status TranscodeManager::Init() {
  ProcessorOptions options = ProcessorOptions::Default();

  if (options_.enable_hd_task) {
    options.name = "hd_processor";
    CG_RETURN_IF_ERROR(TranscodeProcessor::Create(options, &processors_[kHighDefinition]));
  }
  if (options_.enable_nd_task) {
    options.name = "nd_processor";
    CG_RETURN_IF_ERROR(TranscodeProcessor::Create(options, &processors_[kNormalDefinition]));
  }
  if (options_.enable_ld_task) {
    options.name = "ld_processor";
    CG_RETURN_IF_ERROR(TranscodeProcessor::Create(options, &processors_[kLowDefinition]));
  }
  return corgi::Status::OK();
}

////////////////////

corgi::Status TranscodeManager::DoSubmit(DefinitionType type, Task *task) {
  task->OnQueued();
  corgi::Status st = processors_[type]->Submit(task);
  if (!st.ok()) {
    task->OnError("Queued error");
  }
  return st;
}

corgi::Status TranscodeManager::SubmitToHighDefinitionQueue(Task *task) {
  if (!options_.enable_hd_task) {
    return corgi::errors::Unavailable("not enable high definition transcode queue");
  }
  return DoSubmit(kHighDefinition, task);
}

corgi::Status TranscodeManager::SubmitToNormalDefinitionQueue(Task *task) {
  if (!options_.enable_nd_task) {
    return corgi::errors::Unavailable("not enable normal definition transcode queue");
  }
  return DoSubmit(kNormalDefinition, task);
}

corgi::Status TranscodeManager::SubmitToLowDefinitionQueue(Task *task) {
  if (!options_.enable_ld_task) {
    return corgi::errors::Unavailable("not enable low definition transcode queue");
  }
  return DoSubmit(kLowDefinition, task);
}

} // namespace mms
