// GDateTimeImpl.cpp
//
// GDateTime implementation for Windows.


#include <GDateTimeImpl.h>
#include <stdio.h>


// Class GDateTimeImpl

// Constructor
GDateTimeImpl::GDateTimeImpl(void)
{
	// Get current system time.
	GetSystemTime(&m_SystemTime);
}

// Destructor
GDateTimeImpl::~GDateTimeImpl(void)
{
}

// Update
void GDateTimeImpl::Update(void)
{
	// Get current system time.
	GetSystemTime(&m_SystemTime);
}

// GetDateStr
string GDateTimeImpl::GetDateStr(void)
{
	char buf[100];
	sprintf(buf,"%02d-%02d_%02d",
			m_SystemTime.wMonth,
			m_SystemTime.wDay,
			m_SystemTime.wYear);
	return string(buf);
}

// GetTimeStr
string GDateTimeImpl::GetTimeStr(void)
{
	char buf[100];
	sprintf(buf,"%02d:%02d:%02d.%d",
			m_SystemTime.wHour,
			m_SystemTime.wMinute,
			m_SystemTime.wSecond,
			m_SystemTime.wMilliseconds);
	return string(buf);
}
