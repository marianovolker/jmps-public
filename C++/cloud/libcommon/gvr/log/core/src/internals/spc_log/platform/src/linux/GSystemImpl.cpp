// GSystem.cpp
//
// Linux implementation of the GSystem class.

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <stdexcept>
#include <endian.h>
#include <sys/mman.h>
#include <unistd.h>

#include <GLinux.h>
#include <GSystemImpl.h>
#include <GFile.h>

// GSystemImpl

GSystemImpl::TimeStamp::TimeStamp (void)
	:	second(0),
		minute(0),
		hour(0),
		day(0),
		month(0),
		year(0)
{
}

GSystemImpl::TimeStamp::~TimeStamp (void)
{
	second = 0;
	minute = 0;
	hour = 0;
	day = 0;
	month = 0;
	year = 0;
}

bool GSystemImpl::TimeStamp::operator == (GSystemImpl::TimeStamp const & rhs) const
{
	return ((second == rhs.second) && (minute == rhs.minute) && (hour == rhs.hour) && (day == rhs.day) && (month == rhs.month) && (year == rhs.year));
}

bool GSystemImpl::TimeStamp::operator != (GSystemImpl::TimeStamp const & rhs) const
{
	return !((*this) == rhs);
}

GMutex GSystemImpl::m_TraceMutex;

GTaskId GSystemImpl::GetTaskId(void)
{
	return static_cast<GTaskId>(pthread_self());
}

void GSystemImpl::Sleep(unsigned int msec)
{
	::usleep(msec*1000);
}

void GSystemImpl::Yield(void)
{
	sched_yield();
}

bool GSystemImpl::Trace(const char *pMsg)
{
	GMutexLock Lock(m_TraceMutex);
	printf("%s", pMsg);
	fflush(stdout);
	return true;
}

// GetTickCount() - Get running tick count (in ms) since system startup.
unsigned long GSystemImpl::GetTickCount(void)
{
	struct timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
	{
		// Error
		return 0;
	}

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

bool GSystemImpl::RenameFile(const char *pOldPath, const char *pNewPath)
{
	return (rename(pOldPath,pNewPath) == 0);
}

bool GSystemImpl::RemoveFile(const char *pPath)
{
	return (unlink(pPath) == 0);
}

bool GSystemImpl::FileExist(const char *pPath)
{
	struct stat buf;
	return (stat(pPath,&buf) == 0);
}

bool GSystemImpl::MakeDir(const char *pPath)
{
	return ( 0 == mkdir(pPath, S_IRWXU) );
}

GSystemImpl::TimeStamp GSystemImpl::FileTime(const char *pPath)
{
	GSystemImpl::TimeStamp res;
	struct stat buf;
	if(stat(pPath,&buf) == 0)
	{
		struct tm FileTimeStamp;
		localtime_r(&buf.st_mtime, &FileTimeStamp);
		res.second = FileTimeStamp.tm_sec;
		res.minute = FileTimeStamp.tm_min;
		res.hour = FileTimeStamp.tm_hour;
		res.day = FileTimeStamp.tm_mday;
		res.month = FileTimeStamp.tm_mon;
		res.year = FileTimeStamp.tm_year;
	}
	return res;
}

std::list < std::pair <std::string, bool> > GSystemImpl::GetDirEntryList (std::string sPath)
{
	std::list < std::pair <std::string, bool> > lEntryList;

	DIR * pDirectory(NULL);
	struct dirent * pDirEntry (NULL);
	if((pDirectory  = opendir(sPath.c_str())) != NULL)
	{
		while ((pDirEntry = readdir(pDirectory)) != NULL)
		{
			lEntryList.push_back(std::list < std::pair <std::string, bool> >::value_type(pDirEntry->d_name,false));
		}
		closedir(pDirectory);
	}

	return lEntryList;
}

bool GSystemImpl::CopyFile (std::string sSrc, std::string sDst)
{
	GFile oSrc(sSrc);
	GFile oDst(sDst);

	bool bContinue(false);
	if (!oSrc.Open(eOpenReadOnly))
	{
		return false;
		//TODO: LOG? throw std::runtime_error("FileCopy cannot open file to read: " + sSrc);
	}

	if (!oDst.Open(eOpenWriteNew))
	{
		return false;
		//TODO: LOG? throw std::runtime_error("FileCopy cannot open file to write: " + sDst);
	}

	static int const CBufferLength(1024);
	unsigned char ucBuffer[CBufferLength];
	int iBytesRead(0);
	int iTotalBytesWrote(0);
	bContinue = oSrc.Read(ucBuffer,CBufferLength,iBytesRead);
	while ((iBytesRead > 0) && bContinue)
	{
		int iBytesOffset(0);
		while ((iBytesRead > 0) && bContinue)
		{
			int iBytesWrote(0);
			bContinue = oDst.Write(ucBuffer+iBytesOffset, iBytesRead, iBytesWrote);
			if (bContinue == true)
			{
				iBytesRead -= iBytesWrote;
				iBytesOffset += iBytesWrote;
			}
			iTotalBytesWrote += iBytesWrote;
		}
		bContinue = oSrc.Read(ucBuffer,CBufferLength,iBytesRead);
	}

	oDst.Close();
	oSrc.Close();

	return bContinue;
}


// HostIsBigEndian() - Return true if host is big endian.
bool GSystemImpl::HostIsBigEndian(void)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	return true;
#else
	return false;
#endif
}


// HostIsLittleEndian() - Return true if host is little endian.
bool GSystemImpl::HostIsLittleEndian(void)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	return true;
#else
	return false;
#endif
}


bool GSystemImpl::ReadLink(const std::string& sLinkName, std::string& sRealName, int maxLength)
{
	char* realName = new char[maxLength];

	bool isLink = false;
	int len = readlink(sLinkName.c_str(), realName, sizeof(realName) - 1);
	if (len > 0)
	{
		realName[len] = '\0';
		sRealName = realName;
		isLink = true;
	}
	delete [] realName;
	return isLink;
}

struct tm* GSystemImpl::LocalTime(const time_t* pt, struct tm * ptm)
{
	return localtime_r(pt, ptm);
}

void GSystemImpl::Perror(const char *format, va_list args)
{
	char arycbuffer[256];
	memset(arycbuffer, 0, sizeof(arycbuffer));

	vsnprintf(arycbuffer, (sizeof(arycbuffer)-2), format, args);

	fprintf(stderr, "%s\n", arycbuffer);
}
