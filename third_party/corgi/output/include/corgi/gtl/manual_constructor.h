
#ifndef CORGI_GTL_MANUAL_CONSTRUCTOR_H_
#define CORGI_GTL_MANUAL_CONSTRUCTOR_H_

#include <stddef.h>
#include <new>
#include <utility>


#include "corgi/base/macros.h"
#include "corgi/port/mem.h"

namespace corgi {
namespace gtl {
namespace internal {

//
// Provides a char array with the exact same alignment as another type. The
// first parameter must be a complete type, the second parameter is how many
// of that type to provide space for.
//
//   CG_LIB_GTL_ALIGNED_CHAR_ARRAY(struct stat, 16) storage_;
//
// Because MSVC and older GCCs require that the argument to their alignment
// construct to be a literal constant integer, we use a template instantiated
// at all the possible powers of two.
#ifndef SWIG
template <int alignment, int size>
struct AlignType {};
template <int size>
struct AlignType<0, size> {
  typedef char result[size];
};
#if defined(COMPILER_MSVC)
#define CG_LIB_GTL_ALIGN_ATTRIBUTE(X) __declspec(align(X))
#define CG_LIB_GTL_ALIGN_OF(T) __alignof(T)
#elif defined(COMPILER_GCC3) || __GNUC__ >= 3 || defined(__APPLE__) || \
    defined(COMPILER_ICC) || defined(OS_NACL) || defined(__clang__)
#define CG_LIB_GTL_ALIGN_ATTRIBUTE(X) __attribute__((aligned(X)))
#define CG_LIB_GTL_ALIGN_OF(T) __alignof__(T)
#endif

#if defined(CG_LIB_GTL_ALIGN_ATTRIBUTE)

#define CG_LIB_GTL_ALIGNTYPE_TEMPLATE(X)                     \
  template <int size>                                        \
  struct AlignType<X, size> {                                \
    typedef CG_LIB_GTL_ALIGN_ATTRIBUTE(X) char result[size]; \
  }

CG_LIB_GTL_ALIGNTYPE_TEMPLATE(1);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(2);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(4);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(8);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(16);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(32);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(64);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(128);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(256);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(512);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(1024);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(2048);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(4096);
CG_LIB_GTL_ALIGNTYPE_TEMPLATE(8192);
// Any larger and MSVC++ will complain.

#define CG_LIB_GTL_ALIGNED_CHAR_ARRAY(T, Size)                          \
  typename corgi::gtl::internal::AlignType<CG_LIB_GTL_ALIGN_OF(T),      \
                                           sizeof(T) * Size>::result

#undef CG_LIB_GTL_ALIGNTYPE_TEMPLATE
#undef CG_LIB_GTL_ALIGN_ATTRIBUTE

#else  // defined(CG_LIB_GTL_ALIGN_ATTRIBUTE)
#error "You must define CG_LIB_GTL_ALIGNED_CHAR_ARRAY for your compiler."
#endif  // defined(CG_LIB_GTL_ALIGN_ATTRIBUTE)

#else  // !SWIG

// SWIG can't represent alignment and doesn't care about alignment on data
// members (it works fine without it).
template <typename Size>
struct AlignType {
  typedef char result[Size];
};
#define CG_LIB_GTL_ALIGNED_CHAR_ARRAY(T, Size) \
  corgi::gtl::internal::AlignType<Size * sizeof(T)>::result

// Enough to parse with SWIG, will never be used by running code.
#define CG_LIB_GTL_ALIGN_OF(Type) 16

#endif  // !SWIG

}  // namespace internal
}  // namespace gtl

template <typename Type>
class ManualConstructor {
 public:
  // No constructor or destructor because one of the most useful uses of
  // this class is as part of a union, and members of a union cannot have
  // constructors or destructors.  And, anyway, the whole point of this
  // class is to bypass these.

  // Support users creating arrays of ManualConstructor<>s.  This ensures that
  // the array itself has the correct alignment.
  static void* operator new[](size_t size) {
    return port::AlignedMalloc(size, CG_LIB_GTL_ALIGN_OF(Type));
  }
  static void operator delete[](void* mem) { port::AlignedFree(mem); }

  inline Type* get() { return reinterpret_cast<Type*>(space_); }
  inline const Type* get() const {
    return reinterpret_cast<const Type*>(space_);
  }

  inline Type* operator->() { return get(); }
  inline const Type* operator->() const { return get(); }

  inline Type& operator*() { return *get(); }
  inline const Type& operator*() const { return *get(); }

  inline void Init() { new (space_) Type; }

// Init() constructs the Type instance using the given arguments
// (which are forwarded to Type's constructor). In C++11, Init() can
// take any number of arguments of any type, and forwards them perfectly.
// On pre-C++11 platforms, it can take up to 11 arguments, and may not be
// able to forward certain kinds of arguments.
//
// Note that Init() with no arguments performs default-initialization,
// not zero-initialization (i.e it behaves the same as "new Type;", not
// "new Type();"), so it will leave non-class types uninitialized.
#ifdef LANG_CXX11
  template <typename... Ts>
  inline void Init(Ts&&... args) {                 // NOLINT
    new (space_) Type(std::forward<Ts>(args)...);  // NOLINT
  }
#else   // !defined(LANG_CXX11)
  template <typename T1>
  inline void Init(const T1& p1) {
    new (space_) Type(p1);
  }

  template <typename T1, typename T2>
  inline void Init(const T1& p1, const T2& p2) {
    new (space_) Type(p1, p2);
  }

  template <typename T1, typename T2, typename T3>
  inline void Init(const T1& p1, const T2& p2, const T3& p3) {
    new (space_) Type(p1, p2, p3);
  }

  template <typename T1, typename T2, typename T3, typename T4>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4) {
    new (space_) Type(p1, p2, p3, p4);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5) {
    new (space_) Type(p1, p2, p3, p4, p5);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6) {
    new (space_) Type(p1, p2, p3, p4, p5, p6);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7) {
    new (space_) Type(p1, p2, p3, p4, p5, p6, p7);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7, typename T8>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7, const T8& p8) {
    new (space_) Type(p1, p2, p3, p4, p5, p6, p7, p8);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7, typename T8, typename T9>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7, const T8& p8,
                   const T9& p9) {
    new (space_) Type(p1, p2, p3, p4, p5, p6, p7, p8, p9);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7, typename T8, typename T9, typename T10>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7, const T8& p8,
                   const T9& p9, const T10& p10) {
    new (space_) Type(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5,
            typename T6, typename T7, typename T8, typename T9, typename T10,
            typename T11>
  inline void Init(const T1& p1, const T2& p2, const T3& p3, const T4& p4,
                   const T5& p5, const T6& p6, const T7& p7, const T8& p8,
                   const T9& p9, const T10& p10, const T11& p11) {
    new (space_) Type(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
  }
#endif  // LANG_CXX11

  inline void Destroy() { get()->~Type(); }

 private:
  CG_LIB_GTL_ALIGNED_CHAR_ARRAY(Type, 1) space_;
};

#undef CG_LIB_GTL_ALIGNED_CHAR_ARRAY
#undef CG_LIB_GTL_ALIGN_OF

}  // namespace corgi

#endif  // CORGI_GTL_MANUAL_CONSTRUCTOR_H_
