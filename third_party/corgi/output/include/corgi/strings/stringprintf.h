
// Printf variants that place their output in a C++ string.
//
// Usage:
//      string result = strings::Printf("%d %s\n", 10, "hello");
//      strings::SPrintf(&result, "%d %s\n", 10, "hello");
//      strings::Appendf(&result, "%d %s\n", 20, "there");

#ifndef CORGI_STRINGS_STRINGPRINCG_H_
#define CORGI_STRINGS_STRINGPRINCG_H_

#include <stdarg.h>
#include <string>


#include "corgi/base/macros.h"
#include "corgi/base/types.h"

namespace corgi {
namespace strings {

// Return a C++ string
extern string Printf(const char* format, ...)
    // Tell the compiler to do printf format string checking.
    CG_PRINTF_ATTRIBUTE(1, 2);

// Append result to a supplied string
extern void Appendf(string* dst, const char* format, ...)
    // Tell the compiler to do printf format string checking.
    CG_PRINTF_ATTRIBUTE(2, 3);

// Lower-level routine that takes a va_list and appends to a specified
// string.  All other routines are just convenience wrappers around it.
extern void Appendv(string* dst, const char* format, va_list ap);

}  // namespace strings
}  // namespace corgi

#endif  // CORGI_STRINGS_STRINGPRINCG_H_
