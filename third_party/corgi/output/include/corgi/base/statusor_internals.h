
#ifndef CORGI_BASE_STATUSOR_INTERNALS_H_
#define CORGI_BASE_STATUSOR_INTERNALS_H_

#include "corgi/base/status.h"
#include "corgi/base/macros.h"

namespace corgi {
namespace internal_statusor {

class Helper {
 public:
  static void HandleInvalidStatusCtorArg(Status*);
  CG_ATTRIBUTE_NORETURN static void Crash(const Status& status);
};

template <typename T, typename... Args>
void PlacementNew(void* p, Args&&... args) {
#if defined(__GNUC__) && !defined(__clang__)
  // Teach gcc that 'p' cannot be null, fixing code size issues.
  if (p == nullptr) __builtin_unreachable();
#endif
  new (p) T(std::forward<Args>(args)...);
}

template <typename T>
class StatusOrData {
  template <typename U>
  friend class StatusOrData;

 public:
  StatusOrData() = delete;

  StatusOrData(const StatusOrData& other) {
    if (other.ok()) {
      MakeValue(other.data_);
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }

  StatusOrData(StatusOrData&& other) noexcept {
    if (other.ok()) {
      MakeValue(std::move(other.data_));
      MakeStatus();
    } else {
      MakeStatus(std::move(other.status_));
    }
  }

  template <typename U>
  StatusOrData(const StatusOrData<U>& other) {
    if (other.ok()) {
      MakeValue(other.data_);
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }

  template <typename U>
  StatusOrData(StatusOrData<U>&& other) {
    if (other.ok()) {
      MakeValue(std::move(other.data_));
      MakeStatus();
    } else {
      MakeStatus(std::move(other.status_));
    }
  }

  explicit StatusOrData(const T& value) : data_(value) { MakeStatus(); }
  explicit StatusOrData(T&& value) : data_(std::move(value)) { MakeStatus(); }

  explicit StatusOrData(const Status& status) : status_(status) {
    EnsureNotOk();
  }
  explicit StatusOrData(Status&& status) : status_(std::move(status)) {
    EnsureNotOk();
  }

  StatusOrData& operator=(const StatusOrData& other) {
    if (this == &other) return *this;
    if (other.ok())
      Assign(other.data_);
    else
      Assign(other.status_);
    return *this;
  }

  StatusOrData& operator=(StatusOrData&& other) {
    if (this == &other) return *this;
    if (other.ok())
      Assign(std::move(other.data_));
    else
      Assign(std::move(other.status_));
    return *this;
  }

  ~StatusOrData() {
    if (ok()) {
      status_.~Status();
      data_.~T();
    } else {
      status_.~Status();
    }
  }

  void Assign(const T& value) {
    if (ok()) {
      data_.~T();
      MakeValue(value);
    } else {
      MakeValue(value);
      status_ = Status::OK();
    }
  }

  void Assign(T&& value) {
    if (ok()) {
      data_.~T();
      MakeValue(std::move(value));
    } else {
      MakeValue(std::move(value));
      status_ = Status::OK();
    }
  }

  void Assign(const Status& status) {
    Clear();
    status_ = status;
    EnsureNotOk();
  }

  void Assign(Status&& status) {
    Clear();
    status_ = std::move(status);
    EnsureNotOk();
  }

  bool ok() const { return status_.ok(); }

 protected:
  union {
    Status status_;
  };

  struct Dummy {};
  union {
    Dummy dummy_;
    T data_;
  };

  void Clear() {
    if (ok()) data_.~T();
  }

  void EnsureOk() const {
    if (!ok()) Helper::Crash(status_);
  }

  void EnsureNotOk() {
    if (ok()) Helper::HandleInvalidStatusCtorArg(&status_);
  }

  template <typename Arg>
  void MakeValue(Arg&& arg) {
    internal_statusor::PlacementNew<T>(&dummy_, std::forward<Arg>(arg));
  }

  template <typename... Args>
  void MakeStatus(Args&&... args) {
    internal_statusor::PlacementNew<Status>(&status_,
                                            std::forward<Args>(args)...);
  }
};

template <bool Copy, bool Move>
struct TraitsBase {
  TraitsBase() = default;
  TraitsBase(const TraitsBase&) = default;
  TraitsBase(TraitsBase&&) = default;
  TraitsBase& operator=(const TraitsBase&) = default;
  TraitsBase& operator=(TraitsBase&&) = default;
};

template <>
struct TraitsBase<false, true> {
  TraitsBase() = default;
  TraitsBase(const TraitsBase&) = delete;
  TraitsBase(TraitsBase&&) = default;
  TraitsBase& operator=(const TraitsBase&) = delete;
  TraitsBase& operator=(TraitsBase&&) = default;
};

template <>
struct TraitsBase<false, false> {
  TraitsBase() = default;
  TraitsBase(const TraitsBase&) = delete;
  TraitsBase(TraitsBase&&) = delete;
  TraitsBase& operator=(const TraitsBase&) = delete;
  TraitsBase& operator=(TraitsBase&&) = delete;
};

}  // namespace internal_statusor
}  // namespace corgi

#endif  // CORGI_BASE_STATUSOR_INTERNAL
