//
// Created by wangqixiang on 2017/11/23.
//

#include "filesystem/cleaner.h"
#include "corgi/env/env.h"
#include "corgi/subprocess/subprocess.h"

namespace mms {

corgi::Status SimpleCleaner::Cleanup(const std::string &path) {
  corgi::Status status;
  corgi::SubProcess proc;
  proc.SetProgram("/bin/rm", {"rm", "-fr", path});
  proc.SetChannelAction(corgi::CHAN_STDOUT, corgi::ACTION_PIPE);
  proc.SetChannelAction(corgi::CHAN_STDERR, corgi::ACTION_PIPE);
  if (!proc.Start()) {
    return corgi::errors::Internal("cp command error");
  }

  std::string out;
  std::string err;
  int s = proc.Communicate(nullptr, &out, &err);
  if (WEXITSTATUS(status) != 0) {
    return corgi::errors::Internal("cp error: ", err);
  }
  return corgi::Status::OK();
}

} // namespace mss
