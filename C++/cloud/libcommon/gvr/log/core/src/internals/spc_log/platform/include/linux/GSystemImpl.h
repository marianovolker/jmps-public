// GSystem.h
//
// Linux implementation of the GSystem class.

#ifndef GSystemImpl_h
#define GSystemImpl_h


// GSystemImpl
//
// Application interface to system services. All methods
// should be defined as static methods.

#include <GTaskTypes.h>
#include <GMutex.h>
#include <string>
#include <list>

#include <stdarg.h>
#include <string.h>

class GSystemImpl
{
protected:
	struct TimeStamp
	{
		TimeStamp(void);
		~TimeStamp(void);
		unsigned int second;
		unsigned int minute;
		unsigned int hour;
		unsigned int day;
		unsigned int month;
		unsigned int year;
		bool operator == (TimeStamp const & rhs) const;
		bool operator != (TimeStamp const & rhs) const;
	};

	// GetTaskId() - Retrieve ID of calling task.
	static GTaskId GetTaskId(void);

	// Sleep() - Sleep for a period of time.
	static void Sleep(unsigned int msec);

	// Yield() - Yield the CPU allowing other execution.
	static void Yield(void);

	// Trace() - System trace output.
	static bool Trace(const char *pMsg);

	// GetTickCount() - Get running tick count (in ms) since system startup.
	static unsigned long GetTickCount(void);

	// RenameFile() - Rename the corresponding file to the given path/file name.
	static bool RenameFile(const char *pOldPath, const char *pNewPath);

	// RemoveFile() - Removes the corresponding file to the given path/file name.
	static bool RemoveFile(const char *pPath);

	// FileExist() - Checks the file existence to the given path/file name.
	static bool FileExist(const char *pPath);

	// MakeDir() - Creates the specified directory path, returning true if successful
	static bool MakeDir(const char *pPath);

	// FileTime() - Gives the file time from the given path/file name.
	static TimeStamp FileTime(const char *pPath);

	// GetDirEntryList() - Retrieves a file/directory list from the given path name. If the node is a directory it will return true in the second element of the pair.
	static std::list < std::pair <std::string, bool> > GetDirEntryList (std::string sPath);

	//Copy src file contents to the destination
	static bool CopyFile(std::string sSrc, std::string sDst);

	// HostIsBigEndian() - Return true if host is big endian.
	static bool HostIsBigEndian(void);

	// HostIsLittleEndian() - Return true if host is little endian.
	static bool HostIsLittleEndian(void);
	
	// Return true and the link target if sLinkName is a link
	static bool ReadLink(const std::string& sLinkName, std::string& sRealName, int maxLength);

	static struct tm* LocalTime(const time_t* pt, struct tm * ptm);

	static void Perror(const char *format, va_list args);

	// Return
	GSystemImpl(void) { }
	~GSystemImpl(void) { }

private:
	static GMutex m_TraceMutex;

	// Prohibit GSystemImpl copy.
	GSystemImpl(const GSystemImpl &right);
	GSystemImpl& operator=(const GSystemImpl &right);
};

#endif
