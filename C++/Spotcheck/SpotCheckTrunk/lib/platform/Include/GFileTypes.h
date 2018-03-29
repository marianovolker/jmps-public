// GFileTypes.h


#ifndef GFileTypes_h
#define GFileTypes_h

// Define GFile open modes.

typedef enum
{
	eOpenReadOnly,   // Read only
	eOpenReadWrite,  // Read and Write
	eOpenWriteOnly,  // Write only
	eOpenWriteNew,   // Write only, truncate exiting, create if necessary
	eOpenAppend      // Append, create if necessary
} eFileOpenMode;

typedef enum
{
	eSeekStart,
	eSeekCurrent,
	eSeekEnd
} eFileSeekPos;

#endif
