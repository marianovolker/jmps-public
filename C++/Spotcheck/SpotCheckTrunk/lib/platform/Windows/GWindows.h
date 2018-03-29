// GWindows.h

#ifndef GWindows_h
#define GWindows_h

// Windows platform includes.

#include <windows.h>


// Resolve platform name collisions.

#ifdef Yield
#undef Yield
#endif

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#endif
