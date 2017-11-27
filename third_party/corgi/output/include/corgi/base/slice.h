#ifndef CORGI_BASE_SLICE_H_
#define CORGI_BASE_SLICE_H_

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <iosfwd>

#include "corgi/base/types.h"

namespace corgi {

class Slice {
 public:
  typedef size_t size_type;

  Slice() : data_(""), size_(0) {}

  Slice(const char* d, size_t n) : data_(d), size_(n) {}

  Slice(const string& s) : data_(s.data()), size_(s.size()) {}

  Slice(const char* s) : data_(s), size_(strlen(s)) {}

  void set(const void* data, size_t len) {
    data_ = reinterpret_cast<const char*>(data);
	size_ = len;
  }

  const char* data() const { return data_; }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  typedef const char* const_iterator;
  typedef const char* iterator;
  iterator begin() const { return data_; }
  iterator end() const { return data_ + size_; }

  static const size_t npos;

  char operator[](size_t n) const {
    assert(n < size());
	return data_[n];
  }

  void clear() {
    data_ = "";
	size_ = 0;
  }

  void remove_prefix(size_t n) {
    assert(n <= size());
	data_ += n;
	size_ -= n;
  }

  void remove_suffix(size_t n) {
    assert(size_ >= n);
	size_ -= n;
  }

  size_t find(char c, size_t pos=0) const;
  size_t rfind(char c, size_t pos=npos) const;
  bool contains(Slice s) const;

  bool Consume(Slice x) {
    if (starts_with(x)) {
	  remove_prefix(x.size_);
	  return true;
	}
	return false;
  }

  Slice substr(size_t pos, size_t n=npos) const;

  struct Hasher {
    size_t operator()(Slice arg) const;
  };

  string ToString() const { return string(data_, size_); }

  int compare(Slice b) const;

  bool starts_with(Slice x) const {
    return ((size_ >= x.size_) && 
            (memcmp(data_, x.data_, x.size_)==0));
  }

  bool ends_with(Slice x) const {
    return ((size_ >= x.size_) &&
            (memcmp(data_ + (size_ - x.size_), x.data_, x.size_) == 0));
  }

 private:
  const char* data_;
  size_t size_;
};

inline bool operator==(Slice x, Slice y) {
  return ((x.size() == y.size()) &&
          (memcmp(x.data(), y.data(), x.size()) == 0));
}
  
inline bool operator!=(Slice x, Slice y) { return !(x == y  ); }
    
inline bool operator<(Slice x, Slice y) { return x.compare  (y) < 0; }

inline bool operator>(Slice x, Slice y) { return x.compare  (y) > 0; }

inline bool operator<=(Slice x, Slice y) {
  return x.compare(y) <= 0;
}

inline bool operator>=(Slice x, Slice y) {
  return x.compare(y) >= 0;
}
  
inline int Slice::compare(Slice b) const {
  const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
  int r = memcmp(data_, b.data_, min_len);
  if (r == 0) {
    if (size_ < b.size_)
      r = -1; 
	else if (size_ > b.size_)
      r = +1; 
  }
  return r;
}

extern std::ostream& operator<<(std::ostream&, corgi::Slice slice);				              

} // namespace corgi

#endif // CORGI_BASE_SLICE_H_
