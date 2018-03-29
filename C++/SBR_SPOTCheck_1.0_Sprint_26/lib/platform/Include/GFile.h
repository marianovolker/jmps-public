// GFile.h


#ifndef GFile_h
#define GFile_h

#include <GFileTypes.h>
#include <GFileImpl.h>


// GFile
//
// Standard application file interface.

class GFile : private GFileImpl
{
public:
	// Constructor
	GFile(string FileName) : GFileImpl(FileName)
	{
	}

	// Destructor
	virtual ~GFile(void)
	{
	}

	// Open
	bool Open(eFileOpenMode eMode)
	{
		return GFileImpl::Open(eMode);
	}
	
	// Close
	bool Close(void)
	{
		return GFileImpl::Close();
	}
	
	// Read
	bool Read(void *Buf, int Len, int &BytesRead)
	{
		return GFileImpl::Read(Buf, Len, BytesRead);
	}
	
	// Write
	bool Write(const void *Buf, int Len, int &BytesWritten)
	{
		return GFileImpl::Write(Buf, Len, BytesWritten);
	}
	
	/**
	 *
	 * @param ulSize - file size in bytes
	 * @return true on success false otherwise
	 */
	bool Size(unsigned long& ulSize)
	{
		return GFileImpl::Size(ulSize);
	}

	// Tell
	bool Tell(unsigned long &Position)
	{
		return GFileImpl::Tell(Position);
	}

	// Seek
	bool Seek(long Offset, eFileSeekPos Pos)
	{
		return GFileImpl::Seek(Offset, Pos);
	}

	// Rewind
	bool Rewind(void)
	{
		return GFileImpl::Rewind();
	}


private:
	// Prohibit GFile copy.
	GFile(const GFile &right);
	GFile& operator=(const GFile &right);
};


#endif
