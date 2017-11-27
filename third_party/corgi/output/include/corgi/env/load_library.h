
#ifndef CORGI_LOAD_LIBRARY_H_
#define CORGI_LOAD_LIBRARY_H_

#include "corgi/base/status.h"

namespace corgi {

namespace internal {

Status LoadLibrary(const char* library_filename, void** handle);
Status GetSymbolFromLibrary(void* handle, const char* symbol_name,
                            void** symbol);
string FormatLibraryFileName(const string& name, const string& version);

}  // namespace internal
}  // namespace corgi

#endif  // CORGI_LOAD_LIBRARY_H_
