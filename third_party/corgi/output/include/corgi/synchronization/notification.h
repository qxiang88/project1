
#ifndef CORGI_SYNC_NOTIFICATION_H_
#define CORGI_SYNC_NOTIFICATION_H_

#include <assert.h>
#include <chrono>              // NOLINT
#include <condition_variable>  // NOLINT

#include "corgi/synchronization/mutex.h"
#include "corgi/base/types.h"

namespace corgi {

class Notification {
 public:
  Notification() : notified_(false) {}
  ~Notification() {}

  void Notify() {
    mutex_lock l(mu_);
    assert(!notified_);
    notified_ = true;
    cv_.notify_all();
  }

  bool HasBeenNotified() {
    mutex_lock l(mu_);
    return notified_;
  }

  void WaitForNotification() {
    mutex_lock l(mu_);
    while (!notified_) {
      cv_.wait(l);
    }
  }

 private:
  friend bool WaitForNotificationWithTimeout(Notification* n,
                                             int64 timeout_in_us);
  bool WaitForNotificationWithTimeout(int64 timeout_in_us) {
    mutex_lock l(mu_);
    return cv_.wait_for(l, std::chrono::microseconds(timeout_in_us),
                        [this]() { return notified_; });
  }

  mutex mu_;
  condition_variable cv_;
  bool notified_;
};

inline bool WaitForNotificationWithTimeout(Notification* n,
                                           int64 timeout_in_us) {
  return n->WaitForNotificationWithTimeout(timeout_in_us);
}

}  // namespace corgi

#endif  // CORGI_SYNC_NOTIFICATION_H_
