
#ifndef CORGI_BASE_STATUS_H_
#define CORGI_BASE_STATUS_H_

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>

#include "corgi/base/error_codes.h"
#include "corgi/base/slice.h"
#include "corgi/base/macros.h"
#include "corgi/base/logging.h"

namespace corgi {

#if defined(__clang__)
// Only clang supports warn_unused_result as a type annotation.
class CG_MUST_USE_RESULT Status;
#endif

class Status {
 public:
  Status() {}

  Status(Code code, Slice msg);

  Status(const Status& s);
  void operator=(const Status& s);

  static Status OK() { return Status(); }

  bool ok() const { return (state_ == NULL); }

  Code code() const {
    return ok() ? Code::OK : state_->code;
  }

  const string& error_message() const {
    return ok() ? empty_string() : state_->msg;
  }

  bool operator==(const Status& x) const;
  bool operator!=(const Status& x) const;

  void Update(const Status& new_status);

  string ToString() const;

  void IgnoreError() const;

 private:
  static const string& empty_string();
  struct State {
    Code code;
    string msg;
  };
  std::unique_ptr<State> state_;

  void SlowCopyFrom(const State* src);
};

inline Status::Status(const Status& s)
    : state_((s.state_ == NULL) ? NULL : new State(*s.state_)) {}

inline void Status::operator=(const Status& s) {
  if (state_ != s.state_) {
    SlowCopyFrom(s.state_.get());
  }
}

inline bool Status::operator==(const Status& x) const {
  return (this->state_ == x.state_) || (ToString() == x.ToString());
}

inline bool Status::operator!=(const Status& x) const { return !(*this == x); }

std::ostream& operator<<(std::ostream& os, const Status& x);

typedef std::function<void(const Status&)> StatusCallback;

extern corgi::string* CgCheckOpHelperOutOfLine(
    const ::corgi::Status& v, const char* msg);

inline corgi::string* CgCheckOpHelper(::corgi::Status v,
                                      const char* msg) {
  if (v.ok()) return nullptr;
  return CgCheckOpHelperOutOfLine(v, msg);
}

#define CG_DO_CHECK_OK(val, level)                  \
  while (auto _result = CgCheckOpHelper(val, #val)) \
    LOG(level) << *(_result)

#define CG_CHECK_OK(val)  CG_DO_CHECK_OK(val, FATAL)
#define CG_QCHECK_OK(val) CG_DO_CHECK_OK(val, QFATAL)

#ifndef NDEBUG
#define CG_DCHECK_OK(val) CG_CHECK_OK(val)
#else
#define CG_DCHECK_OK(val) \
  while (false && (::corgi::Status::OK() == (val))) LOG(FATAL)
#endif

}  // namespace corgi

#endif  // CORGI_BASE_STATUS_H_
