
#ifndef CORGI_BASE_LOGGING_H_
#define CORGI_BASE_LOGGING_H_

#include <sstream>
#include <limits>
#include "corgi/base/macros.h"
#include "corgi/base/types.h"

#undef ERROR

namespace corgi {

const int INFO = 0;            // base_logging::INFO;
const int WARNING = 1;         // base_logging::WARNING;
const int ERROR = 2;           // base_logging::ERROR;
const int FATAL = 3;           // base_logging::FATAL;
const int NUM_SEVERITIES = 4;  // base_logging::NUM_SEVERITIES;

namespace internal {

class LogMessage : public std::basic_ostringstream<char> {
 public:
  LogMessage(const char* fname, int line, int severity);
  ~LogMessage();

  static int64 MinVLogLevel();

 protected:
  void GenerateLogMessage();

 private:
  const char* fname_;
  int line_;
  int severity_;
};

class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line) CG_ATTRIBUTE_COLD;
  CG_ATTRIBUTE_NORETURN ~LogMessageFatal();
};

#define _CG_LOG_INFO \
  ::corgi::internal::LogMessage(__FILE__, __LINE__, corgi::INFO)
#define _CG_LOG_WARNING \
  ::corgi::internal::LogMessage(__FILE__, __LINE__, corgi::WARNING)
#define _CG_LOG_ERROR \
  ::corgi::internal::LogMessage(__FILE__, __LINE__, corgi::ERROR)
#define _CG_LOG_FATAL \
  ::corgi::internal::LogMessageFatal(__FILE__, __LINE__)

#define _CG_LOG_QFATAL _CG_LOG_FATAL

#define LOG(severity) _CG_LOG_##severity

#define VLOG_IS_ON(lvl) \
  ((lvl) <= ::corgi::internal::LogMessage::MinVLogLevel())

#define VLOG(lvl)                        \
  if (CG_PREDICT_FALSE(VLOG_IS_ON(lvl))) \
  ::corgi::internal::LogMessage(__FILE__, __LINE__, corgi::INFO)

#define CHECK(condition)              \
  if (CG_PREDICT_FALSE(!(condition))) \
  LOG(FATAL) << "Check failed: " #condition " "

template <typename T>
inline const T& GetReferenceableValue(const T& t) {
  return t;
}
inline char GetReferenceableValue(char t) { return t; }
inline unsigned char GetReferenceableValue(unsigned char t) { return t; }
inline signed char GetReferenceableValue(signed char t) { return t; }
inline short GetReferenceableValue(short t) { return t; }
inline unsigned short GetReferenceableValue(unsigned short t) { return t; }
inline int GetReferenceableValue(int t) { return t; }
inline unsigned int GetReferenceableValue(unsigned int t) { return t; }
inline long GetReferenceableValue(long t) { return t; }
inline unsigned long GetReferenceableValue(unsigned long t) { return t; }
inline long long GetReferenceableValue(long long t) { return t; }
inline unsigned long long GetReferenceableValue(unsigned long long t) {
  return t;
}

template <typename T>
inline void MakeCheckOpValueString(std::ostream* os, const T& v) {
  (*os) << v;
}

template <>
void MakeCheckOpValueString(std::ostream* os, const char& v);
template <>
void MakeCheckOpValueString(std::ostream* os, const signed char& v);
template <>
void MakeCheckOpValueString(std::ostream* os, const unsigned char& v);

#if LANG_CXX11
// We need an explicit specialization for std::nullptr_t.
template <>
void MakeCheckOpValueString(std::ostream* os, const std::nullptr_t& p);
#endif

struct CheckOpString {
  CheckOpString(string* str) : str_(str) {}
  operator bool() const { return CG_PREDICT_FALSE(str_ != NULL); }
  string* str_;
};

template <typename T1, typename T2>
string* MakeCheckOpString(const T1& v1, const T2& v2,
                          const char* exprtext) CG_ATTRIBUTE_NOINLINE;

class CheckOpMessageBuilder {
 public:
  explicit CheckOpMessageBuilder(const char* exprtext);
  ~CheckOpMessageBuilder();
  std::ostream* ForVar1() { return stream_; }
  std::ostream* ForVar2();
  string* NewString();

 private:
  std::ostringstream* stream_;
};

template <typename T1, typename T2>
string* MakeCheckOpString(const T1& v1, const T2& v2, const char* exprtext) {
  CheckOpMessageBuilder comb(exprtext);
  MakeCheckOpValueString(comb.ForVar1(), v1);
  MakeCheckOpValueString(comb.ForVar2(), v2);
  return comb.NewString();
}

#define CG_DEFINE_CHECK_OP_IMPL(name, op)                 \
  template <typename T1, typename T2>                     \
  inline string* name##Impl(const T1& v1, const T2& v2,   \
                            const char* exprtext) {       \
    if (CG_PREDICT_TRUE(v1 op v2))                        \
      return NULL;                                        \
    else                                                  \
      return ::corgi::internal::MakeCheckOpString(v1, v2, exprtext); \
  }                                                       \
  inline string* name##Impl(int v1, int v2, const char* exprtext) {  \
    return name##Impl<int, int>(v1, v2, exprtext);                   \
  }                                                                  \
  inline string* name##Impl(const size_t v1, const int v2, const char* exprtext) {       \
    if (CG_PREDICT_FALSE(v2 < 0)) {                                  \
       return ::corgi::internal::MakeCheckOpString(v1, v2, exprtext);\
    }                                                                \
    const size_t uval = (size_t)((unsigned)v1);                      \
    return name##Impl<size_t, size_t>(uval, v2, exprtext);           \
  }                                                                  \
  inline string* name##Impl(const int v1, const size_t v2, const char* exprtext) {       \
    if (CG_PREDICT_FALSE(v2 >= std::numeric_limits<int>::max())) {   \
       return ::corgi::internal::MakeCheckOpString(v1, v2, exprtext);\
    }                                                                \
    const size_t uval = (size_t)((unsigned)v2);                      \
    return name##Impl<size_t, size_t>(v1, uval, exprtext);           \
  }

CG_DEFINE_CHECK_OP_IMPL(Check_EQ,
                        == )  // Compilation error with CHECK_EQ(NULL, x)?
CG_DEFINE_CHECK_OP_IMPL(Check_NE, != )  // Use CHECK(x == NULL) instead.
CG_DEFINE_CHECK_OP_IMPL(Check_LE, <= )
CG_DEFINE_CHECK_OP_IMPL(Check_LT, < )
CG_DEFINE_CHECK_OP_IMPL(Check_GE, >= )
CG_DEFINE_CHECK_OP_IMPL(Check_GT, > )
#undef CG_DEFINE_CHECK_OP_IMPL

#define CHECK_OP_LOG(name, op, val1, val2)                            \
  while (::corgi::internal::CheckOpString _result =              \
             ::corgi::internal::name##Impl(                      \
                 ::corgi::internal::GetReferenceableValue(val1), \
                 ::corgi::internal::GetReferenceableValue(val2), \
                 #val1 " " #op " " #val2))                            \
  ::corgi::internal::LogMessageFatal(__FILE__, __LINE__) << *(_result.str_)

#define CHECK_OP(name, op, val1, val2) CHECK_OP_LOG(name, op, val1, val2)

// CHECK_EQ/NE/...
#define CHECK_EQ(val1, val2) CHECK_OP(Check_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(Check_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(Check_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(Check_LT, <, val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(Check_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(Check_GT, >, val1, val2)
#define CHECK_NOTNULL(val)                                 \
  ::corgi::internal::CheckNotNull(__FILE__, __LINE__, \
                                       "'" #val "' Must be non NULL", (val))

#ifndef NDEBUG
// DCHECK_EQ/NE/...
#define DCHECK(condition) CHECK(condition)
#define DCHECK_EQ(val1, val2) CHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2) CHECK_NE(val1, val2)
#define DCHECK_LE(val1, val2) CHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2) CHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2) CHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2) CHECK_GT(val1, val2)

#else

#define DCHECK(condition) \
  while (false && (condition)) LOG(FATAL)

#define _CG_DCHECK_NOP(x, y) \
  while (false && ((void)(x), (void)(y), 0)) LOG(FATAL)

#define DCHECK_EQ(x, y) _CG_DCHECK_NOP(x, y)
#define DCHECK_NE(x, y) _CG_DCHECK_NOP(x, y)
#define DCHECK_LE(x, y) _CG_DCHECK_NOP(x, y)
#define DCHECK_LT(x, y) _CG_DCHECK_NOP(x, y)
#define DCHECK_GE(x, y) _CG_DCHECK_NOP(x, y)
#define DCHECK_GT(x, y) _CG_DCHECK_NOP(x, y)

#endif

#define QCHECK(condition) CHECK(condition)
#define QCHECK_EQ(x, y) CHECK_EQ(x, y)
#define QCHECK_NE(x, y) CHECK_NE(x, y)
#define QCHECK_LE(x, y) CHECK_LE(x, y)
#define QCHECK_LT(x, y) CHECK_LT(x, y)
#define QCHECK_GE(x, y) CHECK_GE(x, y)
#define QCHECK_GT(x, y) CHECK_GT(x, y)

template <typename T>
T&& CheckNotNull(const char* file, int line, const char* exprtext, T&& t) {
  if (t == nullptr) {
    LogMessageFatal(file, line) << string(exprtext);
  }
  return std::forward<T>(t);
}

}  // namespace internal
}  // namespace corgi

#endif  // CORGI_BASE_LOGGING_H_
