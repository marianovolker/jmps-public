// GTypesImpl.h

#ifndef GTypesImpl_h
#define GTypesImpl_h

// system defined integer sizes.=
#include <stdint.h>

// Basic numeric types.
//
typedef signed char	   INT8;	// 8 bits signed
typedef unsigned char	   UINT8;	// 8 bits unsigned (BYTE)

typedef short              INT16;	// 16 bits signed
typedef unsigned short     UINT16;	// 16 bits unsigned

typedef int32_t            INT32;   // 32 bits signed
typedef uint32_t           UINT32;  // 32 bits unsigned
typedef unsigned int       UINT;
typedef unsigned long      ULONG;

typedef int64_t            INT64;   // 64 bits signed
typedef uint64_t           UINT64;  // 64 bits unsigned

typedef float		   FLOAT;	// Floating point


// Make sure that NULL is defined. 
#ifndef NULL
//	#define NULL 0
#endif


#endif
