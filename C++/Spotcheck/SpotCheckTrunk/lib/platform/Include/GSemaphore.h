// GSemaphore.h

#ifndef GSemaphore_h
#define GSemaphore_h

#include <GSemaphoreImpl.h>


// GSemaphore
//
// Standard application semaphore interface. After construction,
// the semaphore is initialized with the specified number of
// acquisitions available.

class GSemaphore : private GSemaphoreImpl
{
public:
	// Constructor
	GSemaphore(int Count) : GSemaphoreImpl(Count)
	{
	}

	// Destructor
	~GSemaphore(void)
	{
	}

	// Wait() - Wait to acquire the semaphore.
	// params: Timeout = Time to wait in milliseconds, default=forever
	// return: 1 -> Semaphore acquired, 0 -> Semaphore not acquired.
	int Wait(long Timeout = -1)
	{
		return GSemaphoreImpl::Wait(Timeout);
	}

	// Post() - Return acquired semaphore.
	void Post(void)
	{
		GSemaphoreImpl::Post();
	}

private:
	// Prohibit GSempahore copy.
	GSemaphore(const GSemaphore &right);
	GSemaphore& operator=(const GSemaphore &right);
};


#endif
