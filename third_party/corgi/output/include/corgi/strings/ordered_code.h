
#ifndef CORGI_STRINGS_ORDERED_CODE_H__
#define CORGI_STRINGS_ORDERED_CODE_H__

#include <string>
#include "corgi/base/macros.h"
#include "corgi/base/types.h"

namespace corgi {
class Slice;

namespace strings {

class OrderedCode {
 public:
  // -------------------------------------------------------------------
  // Encoding routines: each one of the following routines append
  // one item to "*dest" in an encoding where larger values are
  // ordered lexicographically after smaller values.
  static void WriteString(string* dest, Slice str);
  static void WriteNumIncreasing(string* dest, uint64 num);
  static void WriteSignedNumIncreasing(string* dest, int64 num);

  // -------------------------------------------------------------------
  // Decoding routines: these extract an item earlier encoded using
  // the corresponding WriteXXX() routines above.  The item is read
  // from "*src"; "*src" is modified to point past the decoded item;
  // and if "result" is non-NULL, "*result" is modified to contain the
  // result.  In case of string result, the decoded string is appended to
  // "*result".  Returns true if the next item was read successfully, false
  // otherwise.
  static bool ReadString(Slice* src, string* result);
  static bool ReadNumIncreasing(Slice* src, uint64* result);
  static bool ReadSignedNumIncreasing(Slice* src, int64* result);

  // Helper for testing: corrupt "*str" by changing the kth item separator
  // in the string.
  static void TEST_Corrupt(string* str, int k);

  // Helper for testing.
  // SkipToNextSpecialByte is an internal routine defined in the .cc file
  // with the following semantics. Return a pointer to the first byte
  // in the range "[start..limit)" whose value is 0 or 255.  If no such
  // byte exists in the range, returns "limit".
  static const char* TEST_SkipToNextSpecialByte(const char* start,
                                                const char* limit);

 private:
  // This has only static methods, so disallow construction entirely
  OrderedCode();
  CG_DISALLOW_COPY_AND_ASSIGN(OrderedCode);
};

}  // namespace strings
}  // namespace corgi

#endif  // CORGI_STRINGS_ORDERED_CODE_H__
