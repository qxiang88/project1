//
// Created by wangqixiang on 2017/11/15.
//

#ifndef MEDIA_MANAGE_SYSTEM_TRANSCODE_MANAGER_H
#define MEDIA_MANAGE_SYSTEM_TRANSCODE_MANAGER_H

#include <vector>
#include <deque>
#include <memory>

#include "corgi/base/status.h"
#include "transcode/transcode_processor.h"

namespace mms {

struct TranscodeManagerOptions {
  bool enable_hd_task = false;
  bool enable_nd_task = false;
  bool enable_ld_task = true;

  TranscodeManagerOptions() {}
  ~TranscodeManagerOptions() {}

  static TranscodeManagerOptions Default() {
    return TranscodeManagerOptions();
  }
};

// TODO(xiang88):
// OnQueued()
// OnTranscoding()
// OnTranscodeCompleted()
// GetQueueSize()
//

// 负责管理转码的 Task.
class TranscodeManager {
 public:
  enum DefinitionType {
    kHighDefinition = 0,
    kNormalDefinition = 1,
    kLowDefinition = 2,
    kMaxDefinitionType = 3,
  };

  virtual ~TranscodeManager();

  static corgi::Status Create(const TranscodeManagerOptions& options,
                              std::unique_ptr<TranscodeManager>* result);

  corgi::Status Init();
  corgi::Status SubmitToHighDefinitionQueue(Task* task);
  corgi::Status SubmitToNormalDefinitionQueue(Task* task);
  corgi::Status SubmitToLowDefinitionQueue(Task* task);

  void StopAllProcessors();

 private:
  corgi::Status DoSubmit(DefinitionType type, Task* task);

  TranscodeManager(const TranscodeManagerOptions&);
  TranscodeManagerOptions options_;

  std::array<std::unique_ptr<TranscodeProcessor>,
             kMaxDefinitionType> processors_;

  CG_DISALLOW_COPY_AND_ASSIGN(TranscodeManager);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_TRANSCODE_MANAGER_H
