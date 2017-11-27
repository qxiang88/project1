//
// Created by wangqixiang on 2017/11/22.
//

#ifndef CORGI_INPUTBUFFER_H
#define CORGI_INPUTBUFFER_H

#include <string>
#include "corgi/base/coding.h"
#include "corgi/base/status.h"
#include "corgi/env/env.h"
#include "corgi/base/macros.h"
#include "corgi/base/types.h"

namespace corgi {
namespace io {

class InputBuffer {
 public:
  InputBuffer(RandomAccessFile* file, size_t buffer_bytes);
  ~InputBuffer();

  Status ReadLine(string* result);
  Status ReadNBytes(int64 bytes_to_read, string* result);
  Status ReadNBytes(int64 bytes_to_read, char* result, size_t* bytes_read);

  Status ReadVarint32(uint32* result);
  Status SkipNBytes(int64 bytes_to_skip);
  Status Seek(int64 position);
  int64 Tell() const {
    return file_pos_ - (limit_ - pos_);
  }
  RandomAccessFile* file() const { return file_; }

 private:
  Status FillBuffer();
  Status ReadVarint32Fallback(uint32* result);

  RandomAccessFile* file_;
  int64 file_pos_;
  size_t size_;
  char* buf_;
  char* pos_;
  char* limit_;

  CG_DISALLOW_COPY_AND_ASSIGN(InputBuffer);
};

inline Status InputBuffer::ReadVarint32(uint32* result) {
  if (pos_ + kMaxVarint32Bytes <= limit_) {
    const char* offset = GetVarint32Ptr(pos_, limit_, result);
    if (offset == nullptr) {
	  return errors::OutOfRange("Parsed past limit.");
	}
	pos_ = const_cast<char*>(offset);
	return Status::OK();
  } else {
    return ReadVarint32Fallback(result);
  }
}

} // namespace io
} // namespace corgi
#endif //CORGI_INPUTBUFFER_H
