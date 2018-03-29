/*
 * GStopWatch.cpp
 *
 *  Created on: 24/06/2011
 *      Author: pcrespo
 */

#include "GStopWatchImpl.h"
#include <sys/time.h>
#include <stddef.h>
#include <limits.h>

const unsigned int CMicrosecondsPerMillisecond   = 1000;
const unsigned int CMillisecondsPerSecond   = 1000;


GStopWatchImpl::GStopWatchImpl()
{
	m_StartTime = 0;
	m_StopTime  = 0;
	m_TimerStopped = true;
}

GStopWatchImpl::~GStopWatchImpl()
{

}

void GStopWatchImpl::StartTimer( )
{
	m_TimerStopped = false;
	m_StartTime = CurrentTime();
}

void GStopWatchImpl::StopTimer( )
{
	m_TimerStopped = true;
	m_StopTime = CurrentTime();
}

unsigned int GStopWatchImpl::GetElapsedMilliseconds()
{
	unsigned int elapsedTime = 0;
	unsigned int measuredTime = CurrentTime();

	if(m_TimerStopped)
	{
		measuredTime = m_StopTime;
	}

	if (measuredTime < m_StartTime)
	{
		elapsedTime = (UINT_MAX - m_StartTime) + measuredTime + 1;
	}
	else
	{
		elapsedTime = (measuredTime - m_StartTime);
	}

	return elapsedTime;
}


unsigned int GStopWatchImpl::CurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	unsigned int milliseconds = tv.tv_usec / CMicrosecondsPerMillisecond;
	milliseconds += (tv.tv_sec*CMillisecondsPerSecond);


	return milliseconds;
}
