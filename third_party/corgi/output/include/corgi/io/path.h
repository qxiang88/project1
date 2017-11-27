
#ifndef CORGI_IO_PATH_H_
#define CORGI_IO_PATH_H_


#include "corgi/base/status.h"
#include "corgi/base/slice.h"

namespace corgi {
class Slice;
namespace io {
namespace internal {
string JoinPathImpl(std::initializer_list<Slice> paths);
}

// Utility routines for processing filenames

#ifndef SWIG  // variadic templates
// Join multiple paths together, without introducing unnecessary path
// separators.
// For example:
//
//  Arguments                  | JoinPath
//  ---------------------------+----------
//  '/foo', 'bar'              | /foo/bar
//  '/foo/', 'bar'             | /foo/bar
//  '/foo', '/bar'             | /foo/bar
//
// Usage:
// string path = io::JoinPath("/mydir", filename);
// string path = io::JoinPath(FLAGS_test_srcdir, filename);
// string path = io::JoinPath("/full", "path", "to", "filename);
template <typename... T>
string JoinPath(const T&... args) {
  return internal::JoinPathImpl({args...});
}
#endif /* SWIG */

bool IsAbsolutePath(Slice path);

Slice Dirname(Slice path);

Slice Basename(Slice path);

Slice Extension(Slice path);

string CleanPath(Slice path);

void ParseURI(Slice uri, Slice* scheme, Slice* host,
              Slice* path);

string CreateURI(Slice scheme, Slice host, Slice path);

}  // namespace io
}  // namespace corgi

#endif  // CORGI_IO_PATH_H_
