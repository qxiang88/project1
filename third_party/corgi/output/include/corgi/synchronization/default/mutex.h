
#ifndef CORGI_DEFAULT_MUTEX_H_
#define CORGI_DEFAULT_MUTEX_H_


#include <chrono>
#include <condition_variable>
#include <mutex>

namespace corgi {

#undef mutex_lock

enum LinkerInitialized { LINKER_INITIALIZED };

class mutex : public std::mutex {
 public:
  mutex() {}
  // The default implementation of std::mutex is safe to use after the linker
  // initializations
  explicit mutex(LinkerInitialized x) { (void)x; }

  void lock() { std::mutex::lock(); }
  bool try_lock() {
    return std::mutex::try_lock();
  };
  void unlock() { std::mutex::unlock(); }
};

class mutex_lock : public std::unique_lock<std::mutex> {
 public:
  mutex_lock(class mutex& m) : std::unique_lock<std::mutex>(m) {}
  mutex_lock(class mutex& m, std::try_to_lock_t t)
      : std::unique_lock<std::mutex>(m, t) {}
  mutex_lock(mutex_lock&& ml) noexcept
      : std::unique_lock<std::mutex>(std::move(ml)) {}
  ~mutex_lock() {}
};

// Catch bug where variable name is omitted, e.g. mutex_lock (mu);
#define mutex_lock(x) static_assert(0, "mutex_lock_decl_missing_var_name");

using std::condition_variable;

inline ConditionResult WaitForMilliseconds(mutex_lock* mu,
                                           condition_variable* cv, int64 ms) {
  std::cv_status s = cv->wait_for(*mu, std::chrono::milliseconds(ms));
  return (s == std::cv_status::timeout) ? kCond_Timeout : kCond_MaybeNotified;
}

}  // namespace corgi

#endif  // CORGI_DEFAULT_MUTEX_H_
