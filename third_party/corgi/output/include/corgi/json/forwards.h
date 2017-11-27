
#ifndef JSON_FORWARDS_H_INCLUDED
#define JSON_FORWARDS_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "corgi/json/config.h"
#endif // if !defined(JSON_IS_AMALGAMATION)

namespace corgi {
namespace json {

// writer.h
class FastWriter;
class StyledWriter;

// reader.h
class Reader;

// features.h
class Features;

// value.h
typedef unsigned int ArrayIndex;
class StaticString;
class Path;
class PathArgument;
class Value;
class ValueIteratorBase;
class ValueIterator;
class ValueConstIterator;

} // namespace Json
} // namespace corgi

#endif // JSON_FORWARDS_H_INCLUDED
