
#ifndef CORGI_PLATFORM_POSIX_ERROR_H_
#define CORGI_PLATFORM_POSIX_ERROR_H_


#include "corgi/base/status.h"

namespace corgi {

Status IOError(const string& context, int err_number);

}  // namespace corgi

#endif  // CORGI_PLATFORM_POSIX_POSIX_FILE_SYSTEM_H_
