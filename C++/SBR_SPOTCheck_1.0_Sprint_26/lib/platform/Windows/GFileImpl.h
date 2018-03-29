// GFileImpl.h
//
// GFileImpl class implementation for Windwos.


#ifndef GFileImpl_h
#define GFileImpl_h

#include <GWindows.h>
#include <GFileTypes.h>

#include <string>
using namespace std;

// GFileImpl

class GFileImpl
{
public:
	// Constructor
	GFileImpl(string FileName);

	// Destructor
	virtual ~GFileImpl(void);

	// Open
	bool Open(eFileOpenMode eMode);
	
	// CLose
	bool Close(void);
	
	// Read
	bool Read(void *Buf, int Len, int &BytesRead);
	
	// Write
	bool Write(const void *Buf, int Len, int &BytesWritten);

	// Size
	bool Size(unsigned long& ulSize);

	// Tell
	bool Tell(unsigned long &Position);

	// Seek
	bool Seek(long Offset, eFileSeekPos Pos);

	// Rewind
	bool Rewind(void);

private:
	string m_FileName;
	int    m_FileDescriptor;

	// Prohibit GFileImpl copy.
	GFileImpl(const GFileImpl &right);
	GFileImpl& operator=(const GFileImpl &right);
};


#endif
