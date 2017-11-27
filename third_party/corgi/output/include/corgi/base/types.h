#ifndef CORGI_BASE_TYPES_H_
#define CORGI_BASE_TYPES_H_

#include <string>

#include "corgi/base/integral_types.h"

namespace corgi {

using std::string;

static const uint8 kuint8max = ((uint8)0xFF);
static const uint16 kuint16max = ((uint16)0xFFFF);
static const uint32 kuint32max = ((uint32)0xFFFFFFFF);
static const uint64 kuint64max = ((uint64)0xFFFFFFFFFFFFFFFFull);
static const int8 kint8min = ((int8)~0x7F);
static const int8 kint8max = ((int8)0x7F);
static const int16 kint16min = ((int16)~0x7FFF);
static const int16 kint16max = ((int16)0x7FFF);
static const int32 kint32min = ((int32)~0x7FFFFFFF);
static const int32 kint32max = ((int32)0x7FFFFFFF);
static const int64 kint64min = ((int64)~0x7FFFFFFFFFFFFFFFll);
static const int64 kint64max = ((int64)0x7FFFFFFFFFFFFFFFll);

typedef uint64 Fprint;

} // namespace corgi
#endif // CORGI_BASE_TYPES_H_
