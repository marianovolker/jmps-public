// GSemaphoreImpl.h
//
// Telium implementation of the GSemaphore class.

#ifndef GSemaphoreImpl_h
#define GSemaphoreImpl_h

#include <sys/types.h>
#include <semaphore.h>
#ifdef SPOT_M3 // Excluded by header macros, included by brute force :)
	/* Similar to `sem_wait' but wait only until ABSTIME.  */
	extern "C" int sem_timedwait (sem_t * sem, const struct timespec * abstime);
#endif

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
	sem_t m_Semaphore;

	void TimeSpec(long lmili, struct timespec* stTimeout);

	// Prohibit GSempahore copy.
	GSemaphoreImpl(const GSemaphoreImpl &right);
	GSemaphoreImpl& operator=(const GSemaphoreImpl &right);
};


#endif
