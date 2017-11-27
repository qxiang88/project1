#ifndef CORGI_BASE_REF_COUNTED_H_
#define CORGI_BASE_REF_COUNTED_H_

#include "corgi/base/types.h"
#include "corgi/base/macros.h"
#include "corgi/base/logging.h"

#include <atomic>

namespace corgi {

namespace subtle {

class RefCountedBase {
 protected:
  RefCountedBase();
  ~RefCountedBase();

  void AddRef() {
    DCHECK_GE(ref_count_.load(), 1);
	ref_count_.fetch_add(1, std::memory_order_relaxed);
  }

  bool Release() {
    DCHECK_GT(ref_count_.load(), 0);
	if (RefCountIsOne() || ref_count_.fetch_sub(1) == 1) {
	  DCHECK((ref_count_.store(0), true));
	  // delete this;
	  return true;
	} else {
	  return false;
	}
  }

 private:
  bool RefCountIsOne() const {
    return ref_count_.load(std::memory_order_acquire) == 1;
  }

  mutable std::atomic_int_fast32_t ref_count_;
  CG_DISALLOW_COPY_AND_ASSIGN(RefCountedBase);
};

} // namespace subtle


template <typename T>
class RefCounted : public subtle::RefCountedBase {
 public:
  RefCounted() {}
  ~RefCounted() {}

  void AddRef() {
    subtle::RefCountedBase::AddRef();
  }

  void Release() {
    if (subtle::RefCountedBase::Release()) {
	  delete static_cast<T*>(this);
	}
  }

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
};

template<typename T>
class RefCountedData : public RefCounted<RefCountedData<T>> {
 public:
  RefCountedData() : data() {}
  explicit RefCountedData(const T& in_value) : data(in_value) {}

  T data;
};

} // namespace corgi

template <class T>
class scoped_refptr {
 public:
  scoped_refptr() : ptr_(NULL) {
  }

  scoped_refptr(T* p) : ptr_(p) {
    if (ptr_)
      ptr_->AddRef();
  }
	  
  scoped_refptr(const scoped_refptr<T>& r) : ptr_(r.ptr_) {
    if (ptr_)
      ptr_->AddRef();
  }
		  
  ~scoped_refptr() {
    if (ptr_)
      ptr_->Release();
  }
			  
  T* get() const { return ptr_; }
  operator T*() const { return ptr_; }
  T* operator->() const { return ptr_; }
						  
  scoped_refptr<T>& operator=(T* p) {
    if (p) 
      p->AddRef();
	if (ptr_ )
	  ptr_ ->Release();
	ptr_ = p;
	return *this;
  }
	  
  scoped_refptr<T>& operator=(const scoped_refptr<T>& r) {
    return *this = r.ptr_;
  }
		  
  void swap(T** pp) {
    T* p = ptr_;
    ptr_ = *pp;
    *pp = p;
  }
			  
  void swap(scoped_refptr<T>& r) {
    swap(&r.ptr_);
  }

 protected:
  T* ptr_;
};


#endif // CORGI_BASE_REF_COUNTED_H_
