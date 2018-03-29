/*
 * GStopWatch.cpp
 *
 *  Created on: 24/06/2011
 *      Author: pcrespo
 */

#include "GStopWatchImpl.h"
#include "GWindows.h"

const unsigned int CMillisecondsPerHour   = 3600000;
const unsigned int CMillisecondsPerMinute = 60000;
const unsigned int CMillisecondsPerSecond = 1000;

// Calculate maximum time returned from CurrentTime.
const unsigned int CMaxTime = (23 * CMillisecondsPerHour) +
							  (59 * CMillisecondsPerMinute) +
							  (59 * CMillisecondsPerSecond) +
							  999;


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
		elapsedTime = (CMaxTime - m_StartTime) + measuredTime + 1;
	}
	else
	{
		elapsedTime = (measuredTime - m_StartTime);
	}

	return elapsedTime;
}

unsigned int GStopWatchImpl::CurrentTime()
{
	SYSTEMTIME Now;
	GetSystemTime(&Now);

	return (Now.wHour   * CMillisecondsPerHour   +
			Now.wMinute * CMillisecondsPerMinute +
			Now.wSecond * CMillisecondsPerSecond +
			Now.wMilliseconds);
}
