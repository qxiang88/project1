//
// Created by wangqixiang on 2017/11/22.
//

#ifndef CORGI_RANDOM_INPUTSTREAM_H
#define CORGI_RANDOM_INPUTSTREAM_H

#include "corgi/io/inputstream_interface.h"
#include "corgi/env/file_system.h"

namespace corgi {
namespace io {

// Wraps a RandomAccessFile in an InputStreamInterface. A given instance
// of RandomAccessInputStream is NOT safe for concurrent use by multiple t
// hreads.
class RandomAccessInputStream : public InputStreamInterface {
 public:
  // Does not take ownership of 'file' unless owns_file is set to true.
  // 'file' must outlive *this.
  RandomAccessInputStream(RandomAccessFile* file, bool owns_file = false);

  ~RandomAccessInputStream();

  Status ReadNBytes(int64 bytes_to_read, string* result) override;

  int64 Tell() const override;

  Status Seek(int64 position) {
    pos_ = position;
    return Status::OK();
  }

  Status Reset() override { return Seek(0); }

 private:
  RandomAccessFile* file_;  // Not owned.
  int64 pos_ = 0;           // Tracks where we are in the file.
  bool owns_file_ = false;
};

} // namespace io
} // namespace corgi
#endif //CORGI_RANDOM_INPUTSTREAM_H
