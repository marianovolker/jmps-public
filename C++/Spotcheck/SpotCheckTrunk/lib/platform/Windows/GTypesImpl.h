// GTypesImpl.h

#ifndef GTypesImpl_h
#define GTypesImpl_h

#include <GWindows.h>

// New code being developed should use the bool integral type and
// it's corresponding true and false values.

#ifndef BOOL
	typedef int BOOL;
#endif

// TRUE and FALSE to be used with the BOOL type.
#ifndef TRUE
	#define TRUE  1
#endif
#ifndef FALSE
	#define FALSE 0
#endif

// Basic numeric types.
//
typedef signed char		   INT8;	// 8 bits signed
typedef unsigned char	   UINT8;	// 8 bits unsigned (BYTE)
typedef short              INT16;	// 16 bits signed
typedef unsigned short     UINT16;	// 16 bits unsigned
typedef float			   FLOAT;	// Floating point
/* CTF double
typedef double			   DOUBLE;	// Double
CTF */

typedef unsigned int       UINT;

// Make sure that NULL is defined. 
#ifndef NULL
	#define NULL 0
#endif


#endif
