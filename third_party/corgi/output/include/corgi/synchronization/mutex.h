
#ifndef CORGI_MUTEX_H_
#define CORGI_MUTEX_H_


#include "corgi/base/types.h"

namespace corgi {
enum ConditionResult { kCond_Timeout, kCond_MaybeNotified };
}  // namespace corgi

#include "corgi/synchronization/default/mutex.h"

namespace corgi {

ConditionResult WaitForMilliseconds(mutex_lock* mu, 
                                    condition_variable* cv,
                                    int64 ms);
}  // namespace corgi

#endif  // CORGI_MUTEX_H_
