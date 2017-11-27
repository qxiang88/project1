
#ifndef CORGI_CORE_ERRORS_H_
#define CORGI_CORE_ERRORS_H_

#include "corgi/base/status.h"
#include "corgi/strings/strcat.h"
#include "corgi/base/logging.h"
#include "corgi/base/macros.h"

namespace corgi {
namespace errors {

template <typename... Args>
void AppendToMessage(::corgi::Status* status, Args... args) {
  *status = ::corgi::Status(
      status->code(),
      ::corgi::strings::StrCat(status->error_message(), "\n\t", args...));
}

#define CG_LOG_IF_ERROR(...)                             \
  do {                                                   \
    const ::corgi::Status _status = (__VA_ARGS__);  \
    if (CG_PREDICT_FALSE(!_status.ok())) LOG(ERROR) <<  _status.ToString(); \
  } while (0)
  
#define CG_RETURN_IF_ERROR(...)                          \
  do {                                                   \
    const ::corgi::Status _status = (__VA_ARGS__);  \
    if (CG_PREDICT_FALSE(!_status.ok())) return _status; \
  } while (0)

#define CG_RETURN_WITH_CONTEXT_IF_ERROR(expr, ...)                  \
  do {                                                              \
    ::corgi::Status _status = (expr);                          \
    if (CG_PREDICT_FALSE(!_status.ok())) {                          \
      ::corgi::errors::AppendToMessage(&_status, __VA_ARGS__); \
      return _status;                                               \
    }                                                               \
  } while (0)


#define DECLARE_ERROR(FUNC, CONST)                                 \
  template <typename... Args>                                      \
  ::corgi::Status FUNC(Args... args) {                             \
    return ::corgi::Status(::corgi::Code::CONST,                   \
                           ::corgi::strings::StrCat(args...));     \
  }                                                                \
  inline bool Is##FUNC(const ::corgi::Status& status) {            \
    return status.code() == ::corgi::Code::CONST;                  \
  }

DECLARE_ERROR(Cancelled, CANCELLED)
DECLARE_ERROR(InvalidArgument, INVALID_ARGUMENT)
DECLARE_ERROR(NotFound, NOT_FOUND)
DECLARE_ERROR(AlreadyExists, ALREADY_EXISTS)
DECLARE_ERROR(ResourceExhausted, RESOURCE_EXHAUSTED)
DECLARE_ERROR(Unavailable, UNAVAILABLE)
DECLARE_ERROR(FailedPrecondition, FAILED_PRECONDITION)
DECLARE_ERROR(OutOfRange, OUT_OF_RANGE)
DECLARE_ERROR(Unimplemented, UNIMPLEMENTED)
DECLARE_ERROR(Internal, INTERNAL)
DECLARE_ERROR(Aborted, ABORTED)
DECLARE_ERROR(DeadlineExceeded, DEADLINE_EXCEEDED)
DECLARE_ERROR(DataLoss, DATA_LOSS)
DECLARE_ERROR(Unknown, UNKNOWN)
DECLARE_ERROR(PermissionDenied, PERMISSION_DENIED)
DECLARE_ERROR(Unauthenticated, UNAUTHENTICATED)

#undef DECLARE_ERROR

// The CanonicalCode() for non-errors.
using ::corgi::Code::OK;

}  // namespace errors
}  // namespace corgi

#endif  // CORGI_CORE_ERRORS_H_
