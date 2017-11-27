#ifndef CORGI_PLATFORM_ENV_TIME_H_
#define CORGI_PLATFORM_ENV_TIME_H_

#include <stdint.h>

#include "corgi/base/types.h"

namespace corgi {

class EnvTime {
 public:
  EnvTime();
  virtual ~EnvTime();

  static EnvTime* Default();

  virtual uint64 NowMicros() = 0;

  virtual uint64 NowSeconds() { return NowMicros() / 1000000L; }
};

}  // namespace corgi

#endif  // CORGI_PLATFORM_ENV_TIME_H_
