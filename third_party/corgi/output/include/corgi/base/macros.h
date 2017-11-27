#ifndef CORGI_BASE_MACROS_H_
#define CORGI_BASE_MACROS_H_

// Just for Linux & Macosx

#define CG_ATTRIBUTE_NORETURN      __attribute__((noreturn))
#define CG_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#define CG_ATTRIBUTE_NOINLINE      __attribute__((noinline))
#define CG_ATTRIBUTE_UNUSED        __attribute__((unused))
#define CG_ATTRIBUTE_COLD          __attribute__((cold))
#define CG_ATTRIBUTE_WEAK          __attribute__((weak))
#define CG_PACKED                  __attribute__((packed))
#define CG_MUST_USE_RESULT         __attribute__((warn_unused_result))
#define CG_PRINTF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
#define CG_SCANF_ATTRIBUTE(string_index, first_to_check)  \
  __attribute__((__format__(__scanf__, string_index, first_to_check)))


#define CG_PREDICT_FALSE(x)   (__builtin_expect(x, 0))
#define CG_PREDICT_TRUE(x)    (__builtin_expect(!!(x), 1))

#define CG_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;         \
  void operator=(const TypeName&) = delete

#define CG_ARRAYSIZE(a)                       \
  ((sizeof(a) / sizeof(*(a))) /               \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#ifndef CG_FALLTHROUGH_INTENDED
#define CG_FALLTHROUGH_INTENDED \
  do {                          \
  } while (0)
#endif

#endif // CORGI_BASE_MACROS_H_
