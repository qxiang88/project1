//
// Created by wangqixiang on 2017/11/14.
//

#include "transcode/transcoder.h"
#include "corgi/strings/substitute.h"
#include "corgi/subprocess/subprocess.h"
#include "corgi/env/env.h"


namespace mms {

/////////////////////

Transcoder::Transcoder() {}

Transcoder::~Transcoder() {}

void Transcoder::Reset() {
  commands_.clear();
}

corgi::Status Transcoder::Transcode(Task* task) {
  // 把执行流传回给Task
  // 由Task 负责重组必要的参数, 比如视频的输入文件
  // 输出目标文件, 加密文件等等.
  return task->OnTranscode(this);
}

corgi::Status Transcoder::Stop() {
  return corgi::errors::Unimplemented("Stop");
}

/////////////////////////////////////////////////////
//
//
// 执行真正的转码操作

class FFmpegRunner : public corgi::ProcessObserver {
 public:
  FFmpegRunner(Command command,
               Transcoder::TranscodingStatCB observer = nullptr)
      : command_(command),
        observer_(observer) {}

  corgi::Status Init() {
    ffempg_process_.SetProgram(command_.GetExecutablePath(), command_.ToVector());
    ffempg_process_.SetChannelAction(corgi::CHAN_STDOUT, corgi::ACTION_PIPE);
    ffempg_process_.SetChannelAction(corgi::CHAN_STDERR, corgi::ACTION_PIPE);
    if (observer_) {
      ffempg_process_.SetObserver(this);
    }
    return corgi::Status::OK();
  }

  corgi::Status Run() {
    ffempg_process_.Start();
    int status = ffempg_process_.Communicate(nullptr, nullptr, nullptr);
    if (WEXITSTATUS(status) == 0) {
      return corgi::Status::OK();
    } else {
      return corgi::errors::Internal("ffmpeg error: exist code: ", status);
    }
  }

  // 实现 ProcessorObserver
  virtual void OnStdout(const std::string& message) override {
    observer_(message);
  }
  virtual void OnStderr(const std::string& /* message */) override {
    // TODO(xiang88):
    // 出错信息告诉Task? 还是只通过进程的 status?
  }
 private:
  Command command_;
  Transcoder::TranscodingStatCB observer_;
  corgi::SubProcess ffempg_process_;

  CG_DISALLOW_COPY_AND_ASSIGN(FFmpegRunner);
};

corgi::Status Transcoder::Run(Command command) {
  FFmpegRunner ffmpeg_runner(command,
                             stat_cb_);
  CG_RETURN_IF_ERROR(ffmpeg_runner.Init());
  return ffmpeg_runner.Run();
}

} // namespace mms
