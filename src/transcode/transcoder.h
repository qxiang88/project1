//
// Created by wangqixiang on 2017/11/14.
//

#ifndef TRANSCODER_TRANSCODER_H
#define TRANSCODER_TRANSCODER_H

#include <string>
#include <vector>
#include <functional>

#include "corgi/base/status.h"
#include "task/task.h"

namespace mms {

// Transcoder 负责具体的转码, 加密, 生产M3U8的工作.
// 按理来说我们应该把加密独立出来, 事实上我们也是这么做的. 但是,
// 由于我们的内部实现使用的是 FFmpeg, 而FFmpeg在3版本后直接支持
// HLS AES-128 CBC 加密了, 所以在这里我们暂时把加密的职责也耦合
// 进来.
// TODO(xiang88):
// 把Transcoder 进一步抽象, 分离出FFmpeg做完所有的工作类型, 以及
// FFmpeg 仅完成转码的任务, 加密部分, M3U8的管理部分由不同的实现
// 来负责.
class Transcoder {
 public:
  typedef std::function<void(const std::string& info)> TranscodingStatCB;

  Transcoder();
  virtual ~Transcoder();

  void Reset();

  void SetTranscodingStatCB(TranscodingStatCB cb) {
    stat_cb_ = cb;
  }

  virtual corgi::Status Transcode(Task *task);
  // TODO(xiang88):
  // HDTranscode(Task* task);
  // NDTranscode(Task* task);

  virtual corgi::Status Run(Command command);
  virtual corgi::Status Stop();

 private:
  std::vector<std::string> commands_;
  TranscodingStatCB stat_cb_;

  CG_DISALLOW_COPY_AND_ASSIGN(Transcoder);
};

} // namespace mms
#endif //TRANSCODER_TRANSCODER_H
