// GSemaphoreImpl.cpp
//
// Windows implementation of GSempahore class.


#include <GWindows.h>
#include <GSemaphoreImpl.h>
#include <sys/timeb.h>

static const int MAXSEMVALUE = 99999999;


// GSemaphoreImpl

GSemaphoreImpl::GSemaphoreImpl(int Count)
{
	m_Semaphore = CreateSemaphore(0,Count,MAXSEMVALUE, NULL);
}

GSemaphoreImpl::~GSemaphoreImpl(void)
{
	// Free the semaphore.
	CloseHandle(m_Semaphore);
	m_Semaphore = NULL;
}

int GSemaphoreImpl::Wait(long Timeout)
{
	long windowsTimeout = (Timeout == -1)? INFINITE:Timeout;
	int status = WaitForSingleObject(m_Semaphore, windowsTimeout);

	// Return semaphore acquisition status.
	return (status == 0)?1:0;
}

void GSemaphoreImpl::Post(void)
{
	// Return a count of the semaphore.
	ReleaseSemaphore(m_Semaphore, 1, 0);
}
