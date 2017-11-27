
#ifndef CORGI_GTL_CLEANUP_H_
#define CORGI_GTL_CLEANUP_H_

#include <type_traits>
#include <utility>

#include "corgi/base/macros.h"

namespace corgi {
namespace gtl {

// A move-only RAII object that calls a stored cleanup functor when
// destroyed. Cleanup<F> is the return type of gtl::MakeCleanup(F).
template <typename F>
class Cleanup {
 public:
  Cleanup()
      : released_(true), f_() {}

  template <typename G>
  explicit Cleanup(G&& f)  // NOLINT
      : f_(std::forward<G>(f)) {}  // NOLINT(build/c++11)

  Cleanup(Cleanup&& src)  // NOLINT
      : released_(src.is_released()), f_(src.release()) { }

  // Implicitly move-constructible from any compatible Cleanup<G>.
  // The source will be released as if src.release() were called.
  // A moved-from Cleanup can be safely destroyed or reassigned.
  template <typename G>
  Cleanup(Cleanup<G>&& src)  // NOLINT
      : released_(src.is_released()), f_(src.release()) { }

  // Assignment to a Cleanup object behaves like destroying it
  // and making a new one in its place, analogous to unique_ptr
  // semantics.
  Cleanup& operator=(Cleanup&& src) {  // NOLINT
    if (!released_) f_();
    released_ = src.released_;
    f_ = src.release();
    return *this;
  }

  ~Cleanup() {
    if (!released_) f_();
  }

  // Releases the cleanup function instead of running it.
  // Hint: use c.release()() to run early.
  F release() {
    released_ = true;
    return std::move(f_);
  }

  bool is_released() const { return released_; }

 private:
  static_assert(!std::is_reference<F>::value, "F must not be a reference");

  bool released_ = false;
  F f_;
};

template <int&... ExplicitParameterBarrier,
          typename F, typename DecayF = typename std::decay<F>::type>
CG_MUST_USE_RESULT Cleanup<DecayF> MakeCleanup(F&& f) {
  return Cleanup<DecayF>(std::forward<F>(f));
}

}  // namespace gtl
}  // namespace corgi

#endif  // CORGI_GTL_CLEANUP_H_
