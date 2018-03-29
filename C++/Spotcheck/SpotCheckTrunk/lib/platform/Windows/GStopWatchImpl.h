/*
 * GStopWatchImpl.h
 *
 *  Created on: 24/06/2011
 *      Author: pcrespo
 */

#ifndef GStopWatchImpl_h
#define GStopWatchImpl_h


 class GStopWatchImpl
 {
 public:
	GStopWatchImpl();
	virtual ~GStopWatchImpl();

	void         StartTimer();
	void         StopTimer();
	unsigned int GetElapsedMilliseconds();

 private:
	unsigned int m_StartTime;
	unsigned int m_StopTime;
	bool         m_TimerStopped;

	unsigned int CurrentTime();
};


#endif /* GStopWatchImpl_h */
