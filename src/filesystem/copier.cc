//
// Created by wangqixiang on 2017/11/21.
//

#include "filesystem/copier.h"
#include "corgi/env/env.h"
#include "corgi/subprocess/subprocess.h"

namespace mms {

corgi::Status SimpleCopier::Copy(CopierParameter parameter) {
  corgi::Status status;
  if (parameter.error_if_exist) {
    status = corgi::Env::Default()->IsDirectory(parameter.dst);
    if (status.ok()) {
      return corgi::errors::AlreadyExists("target directory: ", parameter.dst);
    }
  }

  status = corgi::Env::Default()->FileExists(parameter.src);
  if (!status.ok()) {
    return corgi::errors::NotFound("source file/directory: ", parameter.src);
  }

  corgi::SubProcess proc;
  proc.SetProgram("/bin/cp", {"cp", "-fr", parameter.src, parameter.dst});
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

} // namespace mms
