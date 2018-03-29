// GFileImpl.cpp
//
// GFileImpl implementation for Linux.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <GFileImpl.h>
#include <GFile.h>

// GFileImpl

// Constructor
GFileImpl::GFileImpl(string FileName)
         : m_FileName(FileName),
           m_FileDescriptor(-1)
{
}

// Destructor
GFileImpl::~GFileImpl(void)
{
	// Ensure file is close.
	Close();
}

// Open
bool GFileImpl::Open(eFileOpenMode eMode)
{
	int mode = 0;

	if (m_FileDescriptor >= 0)
		return false;

	switch (eMode)
	{
		case eOpenReadOnly:
			mode = O_RDONLY;
			break;

		case eOpenReadWrite:
			mode = O_RDWR;
			break;

		case eOpenWriteOnly:
			mode = O_WRONLY;
			break;

		case eOpenWriteNew:
			mode = O_WRONLY | O_CREAT | O_TRUNC;
			break;

		case eOpenAppend:
			mode = O_WRONLY | O_CREAT | O_APPEND;
			break;

		default:
			return false;
	}

	m_FileDescriptor = open(m_FileName.c_str(), mode, S_IRWXU);
	if (m_FileDescriptor < 0)
		return false;

	return true;
}
	
// Close
bool GFileImpl::Close(void)
{
	if (m_FileDescriptor < 0)
		return false;

	if (close(m_FileDescriptor) != 0)
		return false;

	m_FileDescriptor = -1;

	return true;
}
	
// Read
bool GFileImpl::Read(void *Buf, int Len, int &BytesRead)
{
	if (m_FileDescriptor < 0)
		return false;

	BytesRead = read(m_FileDescriptor, Buf, Len);
	if (BytesRead < 0)
		return false;

	return true;
}
	
// Write
bool GFileImpl::Write(const void *Buf, int Len, int &BytesWritten)
{
	if (m_FileDescriptor < 0)
		return false;

	BytesWritten = write(m_FileDescriptor, Buf, Len);
	if (BytesWritten < 0)
		return false;

	return true;
}

// Size
bool GFileImpl::Size(unsigned long& ulSize)
{
	if (m_FileDescriptor < 0)
		return false;

	struct stat buf;
	if( fstat(m_FileDescriptor, &buf) != 0 )
		return false;

	ulSize = buf.st_size;
	return true;
}

// Tell
bool GFileImpl::Tell(unsigned long &Position)
{
	if (m_FileDescriptor < 0)
		return false;

	Position = lseek(m_FileDescriptor, 0, SEEK_CUR);

	return true;
}

// Seek
bool GFileImpl::Seek(long Offset, eFileSeekPos Pos)
{
	if (m_FileDescriptor < 0)
		return false;

	int Whence = SEEK_CUR;
	switch (Pos)
	{
		case eSeekStart:
			Whence = SEEK_SET;
			break;
		case eSeekCurrent:
			Whence = SEEK_CUR;
			break;
		case eSeekEnd:
			Whence = SEEK_END;
			break;
		default:
			return false;
	}

	if (lseek(m_FileDescriptor,Offset,Whence) < 0)
		return false;

	return true;
}

// Rewind
bool GFileImpl::Rewind(void)
{
	return (Seek(0,eSeekStart));
}

