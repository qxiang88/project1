#ifndef CORGI_STRINGS_STR_UTIL_H_
#define CORGI_STRINGS_STR_UTIL_H_

#include <functional>
#include <string>
#include <vector>

#include "corgi/base/slice.h"
#include "corgi/gtl/array_slice.h"
#include "corgi/strings/strcat.h"
#include "corgi/base/types.h"

// Basic string utility routines
namespace corgi {
namespace str_util {

// Returns a version of 'src' where unprintable characters have been
// escaped using C-style escape sequences.
string CEscape(Slice src);

// Copies "source" to "dest", rewriting C-style escape sequences --
// '\n', '\r', '\\', '\ooo', etc -- to their ASCII equivalents.
//
// Errors: Sets the description of the first encountered error in
// 'error'. To disable error reporting, set 'error' to NULL.
//
// NOTE: Does not support \u or \U!
bool CUnescape(Slice source, string* dest, string* error);

// Removes any trailing whitespace from "*s".
void StripTrailingWhitespace(string* s);

// Removes leading ascii_isspace() characters.
// Returns number of characters removed.
size_t RemoveLeadingWhitespace(Slice* text);

// Removes trailing ascii_isspace() characters.
// Returns number of characters removed.
size_t RemoveTrailingWhitespace(Slice* text);

// Removes leading and trailing ascii_isspace() chars.
// Returns number of chars removed.
size_t RemoveWhitespaceContext(Slice* text);

// Consume a leading positive integer value.  If any digits were
// found, store the value of the leading unsigned number in "*val",
// advance "*s" past the consumed number, and return true.  If
// overflow occurred, returns false.  Otherwise, returns false.
bool ConsumeLeadingDigits(Slice* s, uint64* val);

// Consume a leading token composed of non-whitespace characters only.
// If *s starts with a non-zero number of non-whitespace characters, store
// them in *val, advance *s past them, and return true.  Else return false.
bool ConsumeNonWhitespace(Slice* s, Slice* val);

// If "*s" starts with "expected", consume it and return true.
// Otherwise, return false.
bool ConsumePrefix(Slice* s, Slice expected);

// If "*s" ends with "expected", remove it and return true.
// Otherwise, return false.
bool ConsumeSuffix(Slice* s, Slice expected);

// Return lower-cased version of s.
string Lowercase(Slice s);

// Return upper-cased version of s.
string Uppercase(Slice s);

// Converts "^2ILoveYou!" to "i_love_you_". More specifically:
// - converts all non-alphanumeric characters to underscores
// - replaces each occurence of a capital letter (except the very
//   first character and if there is already an '_' before it) with '_'
//   followed by this letter in lower case
// - Skips leading non-alpha characters
// This method is useful for producing strings matching "[a-z][a-z0-9_]*"
// as required by OpDef.ArgDef.name. The resulting string is either empty or
// matches this regex.
string ArgDefCase(Slice s);

// Capitalize first character of each word in "*s".  "delimiters" is a
// set of characters that can be used as word boundaries.
void TitlecaseString(string* s, Slice delimiters);

// Replaces the first occurrence (if replace_all is false) or all occurrences
// (if replace_all is true) of oldsub in s with newsub.
string StringReplace(Slice s, Slice oldsub, Slice newsub,
                     bool replace_all);

// Join functionality
template <typename T>
string Join(const T& s, const char* sep);

// A variant of Join where for each element of "s", f(&dest_string, elem)
// is invoked (f is often constructed with a lambda of the form:
//   [](string* result, ElemType elem)
template <typename T, typename Formatter>
string Join(const T& s, const char* sep, Formatter f);

struct AllowEmpty {
  bool operator()(Slice sp) const { (void)sp; return true; }
};
struct SkipEmpty {
  bool operator()(Slice sp) const { return !sp.empty(); }
};
struct SkipWhitespace {
  bool operator()(Slice sp) const {
    RemoveTrailingWhitespace(&sp);
    return !sp.empty();
  }
};

// Split strings using any of the supplied delimiters. For example:
// Split("a,b.c,d", ".,") would return {"a", "b", "c", "d"}.
std::vector<string> Split(Slice text, Slice delims);

template <typename Predicate>
std::vector<string> Split(Slice text, Slice delims, Predicate p);

// Split "text" at "delim" characters, and parse each component as
// an integer.  If successful, adds the individual numbers in order
// to "*result" and returns true.  Otherwise returns false.
bool SplitAndParseAsInts(Slice text, char delim,
                         std::vector<int32>* result);
bool SplitAndParseAsInts(Slice text, char delim,
                         std::vector<int64>* result);
bool SplitAndParseAsFloats(Slice text, char delim,
                           std::vector<float>* result);

// ------------------------------------------------------------------
// Implementation details below
template <typename T>
string Join(const T& s, const char* sep) {
  string result;
  bool first = true;
  for (const auto& x : s) {
    corgi::strings::StrAppend(&result, (first ? "" : sep), x);
    first = false;
  }
  return result;
}

template <typename T>
class Formatter {
 public:
  Formatter(std::function<void(string*, T)> f) : f_(f) {}
  void operator()(string* out, const T& t) { f_(out, t); }

 private:
  std::function<void(string*, T)> f_;
};

template <typename T, typename Formatter>
string Join(const T& s, const char* sep, Formatter f) {
  string result;
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      result.append(sep);
    }
    f(&result, x);
    first = false;
  }
  return result;
}

inline std::vector<string> Split(Slice text, Slice delims) {
  return Split(text, delims, AllowEmpty());
}

template <typename Predicate>
std::vector<string> Split(Slice text, Slice delims, Predicate p) {
  std::vector<string> result;
  size_t token_start = 0;
  if (!text.empty()) {
    for (size_t i = 0; i < text.size() + 1; i++) {
      if ((i == text.size()) || (delims.find(text[i]) != Slice::npos)) {
        Slice token(text.data() + token_start, i - token_start);
        if (p(token)) {
          result.push_back(token.ToString());
        }
        token_start = i + 1;
      }
    }
  }
  return result;
}

inline std::vector<string> Split(Slice text, char delim) {
  return Split(text, Slice(&delim, 1));
}

template <typename Predicate>
std::vector<string> Split(Slice text, char delims, Predicate p) {
  return Split(text, Slice(&delims, 1), p);
}

}  // namespace str_util
}  // namespace corgi

#endif  // CORGI_STRINGS_STR_UTIL_H_
