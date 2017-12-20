
#ifndef JSON_AUTOLINK_H_INCLUDED
#define JSON_AUTOLINK_H_INCLUDED

#include "corgi/json/config.h"

#ifdef JSON_IN_CPPTL
#include <cpptl/cpptl_autolink.h>
#endif

#if !defined(JSON_NO_AUTOLINK) && !defined(JSON_DLL_BUILD) &&                  \
    !defined(JSON_IN_CPPTL)
#define CPPTL_AUTOLINK_NAME "json"
#undef CPPTL_AUTOLINK_DLL
#ifdef JSON_DLL
#define CPPTL_AUTOLINK_DLL
#endif
#include "corgi/json/autolink.h"
#endif

#endif // JSON_AUTOLINK_H_INCLUDED