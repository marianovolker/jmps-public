/*
 * GStopWatchImpl.h
 *
 *  Created on: 24/06/2011
 *      Author: pcrespo
 */

#ifndef GStopWatch_h
#define GStopWatch_h

#include "GStopWatchImpl.h"

class GStopWatch : private GStopWatchImpl
{
public:
	GStopWatch(void) : GStopWatchImpl() {}
	virtual ~GStopWatch() {}

	void StartTimer()
	{
		GStopWatchImpl::StartTimer();
	}
	void StopTimer()
	{
		GStopWatchImpl::StopTimer();
	}
	unsigned int GetElapsedMilliseconds()
	{
		return GStopWatchImpl::GetElapsedMilliseconds();
	}
private:
	// Prohibit GStopWatch copy.
	GStopWatch(const GStopWatch &right);
	GStopWatch& operator=(const GStopWatch &right);
};


#endif /* GStopWatch_h */
