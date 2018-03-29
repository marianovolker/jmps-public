// GDateTimeImpl.cpp
//
// GDateTime implementation for Windows.


#include <GDateTimeImpl.h>
#include <stdio.h>


// Class GDateTimeImpl

// Constructor
GDateTimeImpl::GDateTimeImpl(void)
{
	// Get current time.
	gettimeofday(&m_TimeVal, NULL);
}

// Destructor
GDateTimeImpl::~GDateTimeImpl(void)
{
}

// Update
void GDateTimeImpl::Update(void)
{
	// Get current time.
	gettimeofday(&m_TimeVal, NULL);
}

// GetDateStr
string GDateTimeImpl::GetDateStr(void)
{
	struct tm Time;
	localtime_r(&m_TimeVal.tv_sec, &Time);

	char buf[100];
	sprintf(buf,"%02d-%02d-%02d",
			Time.tm_mon + 1,
			Time.tm_mday,
			Time.tm_year + 1900);

	return string(buf);
}

// GetTimeStr
string GDateTimeImpl::GetTimeStr(void)
{
	struct tm Time;
	localtime_r(&m_TimeVal.tv_sec, &Time);

	char buf[100];
	sprintf(buf,"%02d:%02d:%02d.%03d",
			Time.tm_hour,
			Time.tm_min,
			Time.tm_sec,
			static_cast<int>(m_TimeVal.tv_usec/1000));

	return string(buf);
}
