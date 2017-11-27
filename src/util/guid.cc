//
// Created by wangqixiang on 2017/11/18.
//

#include "util/guid.h"

#include "crypto/crypto_util.h"
#include "corgi/strings/stringprintf.h"

namespace mms {

namespace {

bool IsLowerHexDigit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); 
}

bool IsHexDigit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
		 (c >= 'A' && c <= 'F');
}

bool IsValidGUIDInternal(const corgi::Slice& guid, bool strict) {
  const size_t kGUIDLength = 36U;
  if (guid.size() != kGUIDLength) {
    return false;
  }

  for (size_t i = 0; i < guid.size(); ++i) {
    char current = guid[i];
	if (i == 8 || i == 13 || i == 18 || i == 23) {
	  if (current != '-') {
	    return false;
	  } else {
	    if ((strict && !IsLowerHexDigit(current)) || !IsHexDigit(current)) {
		  return false;
		}
	  }
	}
  }
  return true;
}

} // namespace

std::string GenerateGUID() {
  corgi::uint64 sixteen_bytes[2];
  RandBytes(reinterpret_cast<unsigned char *>(&sixteen_bytes), sizeof(sixteen_bytes));  
  sixteen_bytes[0] &= 0xffffffffffff0fffULL;
  sixteen_bytes[0] |= 0x0000000000004000ULL;
  
  sixteen_bytes[1] &= 0x3fffffffffffffffULL;
  sixteen_bytes[1] |= 0x8000000000000000ULL;

  return RandomDataToGUIDString(sixteen_bytes);
}


bool IsValidGUID(const corgi::Slice& guid) {
  return IsValidGUIDInternal(guid, false /* strict */);
}

bool IsValidGUIDOutputString(const corgi::Slice& guid) {
  return IsValidGUIDInternal(guid, true /* strict */);
}

std::string RandomDataToGUIDString(const corgi::uint64 bytes[2]) {
  return corgi::strings::Printf("%08x-%04x-%04x-%04x-%012llx",
	static_cast<unsigned int>(bytes[0] >> 32),
	static_cast<unsigned int>((bytes[0] >> 16) & 0x0000ffff),
    static_cast<unsigned int>(bytes[0] & 0x0000ffff),
    static_cast<unsigned int>(bytes[1] >> 48),
    bytes[1] & 0x0000ffffffffffffULL);
}

} // namespace mms
