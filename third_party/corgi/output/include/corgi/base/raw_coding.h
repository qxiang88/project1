
#ifndef CORGI_CORE_RAW_CODING_H_
#define CORGI_CORE_RAW_CODING_H_

#include <string.h>
#include "corgi/port/cpu_info.h"
#include "corgi/base/types.h"

namespace corgi {

// Lower-level versions of Get... that read directly from a character buffer
// without any bounds checking.

inline uint16 DecodeFixed16(const char* ptr) {
  if (port::kLittleEndian) {
    // Load the raw bytes
    uint16 result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    return ((static_cast<uint16>(static_cast<unsigned char>(ptr[0]))) |
            (static_cast<uint16>(static_cast<unsigned char>(ptr[1])) << 8));
  }
}

inline uint32 DecodeFixed32(const char* ptr) {
  if (port::kLittleEndian) {
    // Load the raw bytes
    uint32 result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    return ((static_cast<uint32>(static_cast<unsigned char>(ptr[0]))) |
            (static_cast<uint32>(static_cast<unsigned char>(ptr[1])) << 8) |
            (static_cast<uint32>(static_cast<unsigned char>(ptr[2])) << 16) |
            (static_cast<uint32>(static_cast<unsigned char>(ptr[3])) << 24));
  }
}

inline uint64 DecodeFixed64(const char* ptr) {
  if (port::kLittleEndian) {
    // Load the raw bytes
    uint64 result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    uint64 lo = DecodeFixed32(ptr);
    uint64 hi = DecodeFixed32(ptr + 4);
    return (hi << 32) | lo;
  }
}

}  // namespace corgi

#endif  // CORGI_CORE_RAW_CODING_H_
