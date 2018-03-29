// GSemaphoreImpl.h
//
// Telium implementation of the GSemaphore class.

#ifndef GSemaphoreImpl_h
#define GSemaphoreImpl_h

#include "GWindows.h"

// GSemaphoreImpl

class GSemaphoreImpl
{
protected:
	// Constructor
	GSemaphoreImpl(int Count);

	// Destructor
	virtual ~GSemaphoreImpl(void);

	// Wait() - Wait to acquire the semaphore.
	int  Wait(long Timeout);

	// Post() - Return acquired semaphore.
	void Post(void);

private:
	HANDLE m_Semaphore;

	void TimeSpec(long lmili, struct timespec* stTimeout);

	// Prohibit GSempahore copy.
	GSemaphoreImpl(const GSemaphoreImpl &right);
	GSemaphoreImpl& operator=(const GSemaphoreImpl &right);
};


#endif
