//	=============================================================================
//
//	File: GMemFile.h
//
//	Description: Dynamic memory implementation with file based backup.
//
//	Created: 1/2/2007
//
//	Author: Chris Fleming
//
//	=============================================================================


#ifndef GMemFile_h
#define GMemFile_h 1

#include "GTypes.h"
#include "GTimer.h"

#include <string>

// Define autosave timer value.

#define MEMFILE_AUTOSAVE_TIME 60000



// Class definition for GMemFile
class GMemFile
{
public:
	// GMemFile Constructor - Params: Unique memory name and size to allocate.
	GMemFile(const char *Name,int Size);
	~GMemFile();

	// Ge tthe raw pointer of allocated memory.
   void  *GetMemPtr();

	// Application initiated file image update.
   void  Save();

private:
   std::string m_FilePath;
   int   m_Size;

   static GMutex m_Mutex;

	void   *m_pMem;
	GTimer *m_pTimer;
   
	bool  Load();		// Initial load of file image into allocated memory area.
	void  Dump();		// Autosave routine called periodicall based on the timer.
	void  Backup();	// Create backup copy of current image.
	void  Restore();	// Restore backup copy as current image.
};


#endif
