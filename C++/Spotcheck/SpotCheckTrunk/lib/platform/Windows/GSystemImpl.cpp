// GSystem.cpp
//
// Telium implementation of the GSystem class.

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <algorithm>
#include <stdexcept>

#include "GWindows.h"
#include "GSystemImpl.h"
#include "GFile.h"

#include <stdarg.h>
#include <string.h>

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
	GTaskId gTaskId(eInvalidTaskId);
	{
		DWORD dwTaskId(::GetCurrentThreadId());
		if (dwTaskId != 0)
		{
			gTaskId = static_cast<GTaskId>(dwTaskId);
		}
	}
	return gTaskId;
}

void GSystemImpl::Sleep(unsigned int msec)
{
	::Sleep(msec);
}

void GSystemImpl::Yield(void)
{
	::Sleep(0);
}

bool GSystemImpl::Trace(const char *pMsg)
{
	GMutexLock Lock(m_TraceMutex);
	printf(pMsg);
	fflush(stdout);
	return true;
}

// GetTickCount() - Get running tick count (in ms) since system startup.
unsigned long GSystemImpl::GetTickCount(void)
{
	return ::GetTickCount();
}

bool GSystemImpl::RenameFile(const char *pOldPath, const char *pNewPath)
{
	return (rename(pOldPath,pNewPath) == 0);
}

bool GSystemImpl::RemoveFile(const char *pPath)
{
	std::string windowsStypePath = GSystemImpl::ConvertPathToWindowsStyle(pPath);
	return (unlink(windowsStypePath.c_str()) == 0);
}

bool GSystemImpl::FileExist(const char *pPath)
{
	struct stat buf;
	return (stat(pPath,&buf) == 0);
}

bool GSystemImpl::MakeDir(const char *pPath)
{
	return CreateDirectory(pPath, NULL)?true:false;
}

GSystemImpl::TimeStamp GSystemImpl::FileTime(const char *pPath)
{
	GSystemImpl::TimeStamp res;
	struct stat buf;
	if(stat(pPath,&buf) == 0)
	{
		struct tm * pFileTimeStamp;
		pFileTimeStamp = localtime(&buf.st_mtime);
		if (pFileTimeStamp != NULL)
		{
			res.second = pFileTimeStamp->tm_sec;
			res.minute = pFileTimeStamp->tm_min;
			res.hour = pFileTimeStamp->tm_hour;
			res.day = pFileTimeStamp->tm_mday;
			res.month = pFileTimeStamp->tm_mon;
			res.year = pFileTimeStamp->tm_year;
		}
	}
	return res;
}

std::list < std::pair <std::string, bool> > GSystemImpl::GetDirEntryList (std::string sPath)
{
	std::list < std::pair <std::string, bool> > lEntryList;
	WIN32_FIND_DATA FindFileData;
	std::string windowsStypePath = GSystemImpl::ConvertPathToWindowsStyle(sPath);
	if( windowsStypePath[windowsStypePath.size()-1] != '\\') windowsStypePath += "\\";
	
	windowsStypePath += "*";
	HANDLE hFind = FindFirstFile(windowsStypePath.c_str(), &FindFileData);

	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			lEntryList.push_back(std::list < std::pair <std::string, bool> >::value_type(FindFileData.cFileName,false));
		}
		while (FindNextFile(hFind, &FindFileData) != 0);	
	}

	return lEntryList;
}

std::string GSystemImpl::ConvertPathToWindowsStyle(std::string sPath)
{
	std::string windowsStypePath(sPath);
	std::replace(windowsStypePath.begin(), windowsStypePath.end(), '/', '\\');

	return windowsStypePath;
}


bool GSystemImpl::CopyFile (std::string sSrc, std::string sDst)
{
	GFile oSrc(GSystemImpl::ConvertPathToWindowsStyle(sSrc));

	GFile oDst(GSystemImpl::ConvertPathToWindowsStyle(sDst));

	bool bContinue(false);
	if (!oSrc.Open(eOpenReadOnly)) 
	{
		return false;
		//TODO: LOG? throw std::runtime_error("FileCopy cannot open file to read: " + GSystemImpl::ConvertPathToWindowsStyle(sSrc));
	}

	if (!oDst.Open(eOpenWriteNew))
	{
		return false;
		//TODO: LOG? throw std::runtime_error("FileCopy cannot open file to write: " + GSystemImpl::ConvertPathToWindowsStyle(sDst));
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
    return false;
}


// HostIsLittleEndian() - Return true if host is little endian.
bool GSystemImpl::HostIsLittleEndian(void)
{
    return true;
}


bool GSystemImpl::ReadLink(const std::string& sLinkName, std::string& sRealName, int maxLength)
{
	(void)sLinkName;
	(void)sRealName;
	(void)maxLength;
	return false;
}

struct tm* GSystemImpl::LocalTime(const time_t* pt, struct tm * ptm)
{
	static GMutex AuxWinLocaltimeMutex;

	AuxWinLocaltimeMutex.Lock();

	// original struct is stored in static buffer into glibc !!!
	// local stack copy is secured by mutex protection
	*ptm = * localtime(pt);

	AuxWinLocaltimeMutex.Unlock();

	return ptm;
}

void GSystemImpl::Perror(const char *format, va_list args)
{
	char arycbuffer[256];

	memset(arycbuffer, 0, sizeof(arycbuffer));

	vsnprintf(arycbuffer, (sizeof(arycbuffer)-1), format, args);

	perror(arycbuffer);
}
