#ifndef CORGI_ENV_FILE_STATISTICS_H_
#define CORGI_ENV_FILE_STATISTICS_H_

#include "corgi/base/types.h"

namespace corgi {

struct FileStatistics {
  int64 length = -1;
  int64 mtime_nsec = 0;
  bool is_directory = false;

  FileStatistics() {}
  FileStatistics(int64 length,
                 int64 mtime_nsec,
				 bool is_directory)
    : length(length),
      mtime_nsec(mtime_nsec),
      is_directory(is_directory) {}
  ~FileStatistics() {}
};


} // namespace corgi

#endif // CORGI_ENV_FILE_STATISTICS_H_
