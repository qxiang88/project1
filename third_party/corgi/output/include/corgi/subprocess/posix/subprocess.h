
#ifndef CORGI_SUBPROCESS_POSIX_SUBPROCESS_H_
#define CORGI_SUBPROCESS_POSIX_SUBPROCESS_H_

#include <errno.h>
#include <unistd.h>

#include <string>
#include <vector>


#include "corgi/base/macros.h"
#include "corgi/base/types.h"
#include "corgi/synchronization/mutex.h"

namespace corgi {

class ProcessObserver {
 public:
  virtual ~ProcessObserver() = default;

  virtual void OnStdout(const std::string& message) = 0;
  virtual void OnStderr(const std::string& message) = 0;
};

class SubProcess {
 public:
  explicit SubProcess(int nfds = 3);

  virtual ~SubProcess();

  virtual void SetChannelAction(Channel chan, 
				                ChannelAction action);
  virtual void SetProgram(const string& file, 
				          const std::vector<string>& argv);
  virtual void SetObserver(ProcessObserver* observer);

  virtual bool Start();

  virtual bool Kill(int signal);

  virtual bool Wait();

  virtual int Communicate(const string* stdin_input, 
				          string* stdout_output,
                          string* stderr_output);

  
 private:
  static const int kNFds = 3;
  static bool chan_valid(int chan) { return ((chan >= 0) && (chan < kNFds)); }
  static bool retry(int e) {
    return ((e == EINTR) || (e == EAGAIN) || (e == EWOULDBLOCK));
  }
  void FreeArgs();
  void ClosePipes();
  bool WaitInternal(int* status);

  mutable mutex proc_mu_;
  bool running_;
  pid_t pid_;

  mutable mutex data_mu_;
  char* exec_path_;
  char** exec_argv_;
  ChannelAction action_[kNFds];
  int parent_pipe_[kNFds];
  int child_pipe_[kNFds];

  ProcessObserver* observer_;

  CG_DISALLOW_COPY_AND_ASSIGN(SubProcess);
};

}  // namespace corgi

#endif  // CORGI_SUBPROCESS_POSIX_SUBPROCESS_H_
