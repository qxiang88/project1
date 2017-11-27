
#ifndef CORGI_SUBPROCESS_SUBPROCESS_H_
#define CORGI_SUBPROCESS_SUBPROCESS_H_

namespace corgi {

enum Channel {
  CHAN_STDIN = 0,
  CHAN_STDOUT = 1,
  CHAN_STDERR = 2,
};

enum ChannelAction {
  ACTION_CLOSE,
  ACTION_PIPE,
  ACTION_DUPPARENT,
};

class SubProcess;

}  // namespace corgi 

#include "corgi/subprocess/posix/subprocess.h"

#endif  // CORGI_SUBPROCESS_SUBPROCESS_H_
