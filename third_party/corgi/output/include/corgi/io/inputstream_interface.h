//
// Created by wangqixiang on 2017/11/22.
//

#ifndef CORGI_INPUTSTREAM_INTERFACE_H
#define CORGI_INPUTSTREAM_INTERFACE_H

#include <string>

#include "corgi/base/status.h"
#include "corgi/base/types.h"

namespace corgi {
namespace io {

class InputStreamInterface {
 public:
  InputStreamInterface() {}
  virtual ~InputStreamInterface() {}

  virtual Status ReadNBytes(int64 bytes_to_read, string* result) = 0;
  virtual Status SkipNBytes(int64 bytes_to_skip);
  virtual int64 Tell() const = 0;
  virtual Status Reset() = 0;
};

} // namespace io
} // namespace corgi
#endif //CORGI_INPUTSTREAM_INTERFACE_H
