
#ifndef CORGI_ENV_RETRYING_UTILS_H_
#define CORGI_ENV_RETRYING_UTILS_H_

#include <functional>
#include "corgi/base/status.h"

namespace corgi {

class RetryingUtils {
 public:
  static Status CallWithRetries(const std::function<Status()>& f,
                                const int64 initial_delay_microseconds);
  static Status CallWithRetries(const std::function<Status()>& f,
                                const int64 initial_delay_microseconds,
                                const std::function<void(int64)>& sleep_usec);
  static Status DeleteWithRetries(const std::function<Status()>& delete_func,
                                  const int64 initial_delay_microseconds);
};

}  // namespace corgi

#endif  // CORGI_ENV_RETRYING_UTILS_H_
