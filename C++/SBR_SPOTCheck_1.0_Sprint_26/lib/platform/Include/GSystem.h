// GSystem.h

#ifndef GSystem_h
#define GSystem_h

// GSystem
//
// Application interface to system services. All methods
// should be defined as static methods.

#include <GSystemImpl.h>

class GSystem
	:	private GSystemImpl
{
public:
	using GSystemImpl::TimeStamp;

	// GetTaskId() - Retrieve ID of calling task.
	static GTaskId GetTaskId(void)
	{
		return GSystemImpl::GetTaskId();
	}

	// Sleep() - Sleep for a period of time.
	static void Sleep(unsigned int msec)
	{
		GSystemImpl::Sleep(msec);
	}

	// Yield() - Yield the CPU allowing other execution.
	static void Yield(void)
	{
		GSystemImpl::Yield();
	}

	// Trace() - System trace output.
	static bool Trace(const char *pMsg)
	{
		return GSystemImpl::Trace(pMsg);
	}

	// GetTickCount() - Get running tick count (in ms) since system startup.
	static unsigned long GetTickCount(void)
	{
		return GSystemImpl::GetTickCount();
	}

	static bool RenameFile(const char *pOldPath, const char *pNewPath)
	{
		return GSystemImpl::RenameFile(pOldPath,pNewPath);
	}

	static bool RemoveFile(const char *pPath)
	{
		return GSystemImpl::RemoveFile(pPath);
	}

	static bool FileExist(const char *pPath)
	{
		return GSystemImpl::FileExist(pPath);
	}

	// MakeDir() - Creates the specified directory path, returning true if successful
	static bool MakeDir(const char *pPath)
	{
		return GSystemImpl::MakeDir(pPath);
	}
	
	static TimeStamp FileTime(const char *pPath)
	{
		return GSystemImpl::FileTime(pPath);
	}

	static std::list < std::pair <std::string, bool> > GetDirEntryList (std::string sPath)
	{
		return GSystemImpl::GetDirEntryList(sPath);
	}

	static bool CopyFile(std::string sSrc, std::string sDst)
	{
		return GSystemImpl::CopyFile(sSrc, sDst);
	}

	static bool HostIsBigEndian(void)
	{
		return GSystemImpl::HostIsBigEndian();
	}

	static bool HostIsLittleEndian(void)
	{
		return GSystemImpl::HostIsLittleEndian();
	}
	
	// If the given name, sLinkName is a link, this method returns true and
	// returns the link target name in sRealName.  If sLinkName is not a link,
	// the method return false and sRealName is untouched.
	static bool ReadLink(const std::string& sLinkName, std::string& sRealName, int maxLength)
	{
		return GSystemImpl::ReadLink(sLinkName, sRealName, maxLength);
	}

	static struct tm* LocalTime(const time_t* pt, struct tm * ptm)
	{
		return GSystemImpl::LocalTime(pt, ptm);
	}

	static void Perror(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		GSystemImpl::Perror(format, args);
		va_end(args);
	}


private:
	// Prohibit construction.
	GSystem(void)
		: GSystemImpl()
	{}
	~GSystem(void) { }

	// Prohibit GSystem copy.
	GSystem(const GSystem &right);
	GSystem& operator=(const GSystem &right);
};

#endif
