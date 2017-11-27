
#ifndef CORGI_STRINGS_SCANNER_H_
#define CORGI_STRINGS_SCANNER_H_

#include <string>

#include "corgi/base/slice.h"
#include "corgi/base/macros.h"

namespace corgi {
namespace strings {

class Scanner {
 public:
  enum CharClass {
    ALL,
    DIGIT,
    LETTER,
    LETTER_DIGIT,
    LETTER_DIGIT_DASH_UNDERSCORE,
    LETTER_DIGIT_DASH_DOT_SLASH,             // SLASH is / only, not backslash
    LETTER_DIGIT_DASH_DOT_SLASH_UNDERSCORE,  // SLASH is / only, not backslash
    LETTER_DIGIT_DOT,
    LETTER_DIGIT_DOT_PLUS_MINUS,
    LETTER_DIGIT_DOT_UNDERSCORE,
    LETTER_DIGIT_UNDERSCORE,
    LOWERLETTER,
    LOWERLETTER_DIGIT,
    LOWERLETTER_DIGIT_UNDERSCORE,
    NON_ZERO_DIGIT,
    SPACE,
    UPPERLETTER,
  };

  explicit Scanner(Slice source) : cur_(source) { RestartCapture(); }

  Scanner& One(CharClass clz) {
    if (cur_.empty() || !Matches(clz, cur_[0])) {
      return Error();
    }
    cur_.remove_prefix(1);
    return *this;
  }

  Scanner& ZeroOrOneLiteral(Slice s) {
    cur_.Consume(s);
    return *this;
  }

  Scanner& OneLiteral(Slice s) {
    if (!cur_.Consume(s)) {
      error_ = true;
    }
    return *this;
  }

  Scanner& Any(CharClass clz) {
    while (!cur_.empty() && Matches(clz, cur_[0])) {
      cur_.remove_prefix(1);
    }
    return *this;
  }

  Scanner& Many(CharClass clz) { return One(clz).Any(clz); }

  Scanner& RestartCapture() {
    capture_start_ = cur_.data();
    capture_end_ = nullptr;
    return *this;
  }

  Scanner& StopCapture() {
    capture_end_ = cur_.data();
    return *this;
  }

  Scanner& Eos() {
    if (!cur_.empty()) error_ = true;
    return *this;
  }

  Scanner& AnySpace() { return Any(SPACE); }

  Scanner& ScanUntil(char end_ch) {
    ScanUntilImpl(end_ch, false);
    return *this;
  }

  Scanner& ScanEscapedUntil(char end_ch) {
    ScanUntilImpl(end_ch, true);
    return *this;
  }

  char Peek(char default_value = '\0') const {
    return cur_.empty() ? default_value : cur_[0];
  }

  int empty() const { return cur_.empty(); }

  bool GetResult(Slice* remaining = nullptr,
                 Slice* capture = nullptr);

 private:
  void ScanUntilImpl(char end_ch, bool escaped);

  Scanner& Error() {
    error_ = true;
    return *this;
  }

  static bool IsLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  static bool IsLowerLetter(char ch) { return ch >= 'a' && ch <= 'z'; }

  static bool IsDigit(char ch) { return ch >= '0' && ch <= '9'; }

  static bool IsSpace(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' ||
            ch == '\r');
  }

  static bool Matches(CharClass clz, char ch) {
    switch (clz) {
      case ALL:
        return true;
      case DIGIT:
        return IsDigit(ch);
      case LETTER:
        return IsLetter(ch);
      case LETTER_DIGIT:
        return IsLetter(ch) || IsDigit(ch);
      case LETTER_DIGIT_DASH_UNDERSCORE:
        return (IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '_');
      case LETTER_DIGIT_DASH_DOT_SLASH:
        return IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '.' ||
               ch == '/';
      case LETTER_DIGIT_DASH_DOT_SLASH_UNDERSCORE:
        return (IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '.' ||
                ch == '/' || ch == '_');
      case LETTER_DIGIT_DOT:
        return IsLetter(ch) || IsDigit(ch) || ch == '.';
      case LETTER_DIGIT_DOT_PLUS_MINUS:
        return IsLetter(ch) || IsDigit(ch) || ch == '+' || ch == '-' ||
               ch == '.';
      case LETTER_DIGIT_DOT_UNDERSCORE:
        return IsLetter(ch) || IsDigit(ch) || ch == '.' || ch == '_';
      case LETTER_DIGIT_UNDERSCORE:
        return IsLetter(ch) || IsDigit(ch) || ch == '_';
      case LOWERLETTER:
        return ch >= 'a' && ch <= 'z';
      case LOWERLETTER_DIGIT:
        return IsLowerLetter(ch) || IsDigit(ch);
      case LOWERLETTER_DIGIT_UNDERSCORE:
        return IsLowerLetter(ch) || IsDigit(ch) || ch == '_';
      case NON_ZERO_DIGIT:
        return IsDigit(ch) && ch != '0';
      case SPACE:
        return IsSpace(ch);
      case UPPERLETTER:
        return ch >= 'A' && ch <= 'Z';
    }
    return false;
  }

  Slice cur_;
  const char* capture_start_ = nullptr;
  const char* capture_end_ = nullptr;
  bool error_ = false;

  friend class ScannerTest;
  CG_DISALLOW_COPY_AND_ASSIGN(Scanner);
};

}  // namespace strings
}  // namespace corgi

#endif  // CORGI_STRINGS_SCANNER_H_
