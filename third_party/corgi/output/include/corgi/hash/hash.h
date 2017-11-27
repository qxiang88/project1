
// Simple hash functions used for internal data structures

#ifndef CORGI_HASH_HASH_H_
#define CORGI_HASH_HASH_H_

#include <stddef.h>
#include <stdint.h>

#include <string>


#include "corgi/base/slice.h"
#include "corgi/base/types.h"

namespace corgi {
namespace hash {

extern uint32 Hash32(const char* data, size_t n, uint32 seed);
extern uint64 Hash64(const char* data, size_t n, uint64 seed);

inline uint64 Hash64(const char* data, size_t n) {
  return Hash64(data, n, 0xDECAFCAFFE);
}

inline uint64 Hash64(const string& str) {
  return Hash64(str.data(), str.size());
}

inline uint64 Hash64Combine(uint64 a, uint64 b) {
  return a ^ (b + 0x9e3779b97f4a7800ULL + (a << 10) + (a >> 4));
}

template <typename T>
struct hash {
  size_t operator()(const T& t) const { return std::hash<T>()(t); }
};

template <typename T>
struct hash<T*> {
  size_t operator()(const T* t) const {
    size_t k = static_cast<size_t>(reinterpret_cast<uintptr_t>(t));
    return k + (k >> 6);
  }
};

template <>
struct hash<string> {
  size_t operator()(const string& s) const {
    return static_cast<size_t>(Hash64(s));
  }
};

template <>
struct hash<Slice> {
  size_t operator()(Slice sp) const {
    return static_cast<size_t>(Hash64(sp.data(), sp.size()));
  }
};

template <typename T, typename U>
struct hash<std::pair<T, U>> {
  size_t operator()(const std::pair<T, U>& p) const {
    return Hash64Combine(hash<T>()(p.first), hash<U>()(p.second));
  }
};

} // namespace hash
} // namespace corgi

#endif  // CORGI_HASH_HASH_H_
