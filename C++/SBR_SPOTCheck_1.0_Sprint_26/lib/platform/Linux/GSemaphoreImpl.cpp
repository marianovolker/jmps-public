// GSemaphoreImpl.cpp
//
// Windows implementation of GSempahore class.


#include <GSemaphoreImpl.h>
#include <sys/timeb.h>
#include <errno.h>

// GSemaphoreImpl

GSemaphoreImpl::GSemaphoreImpl(int Count)
{
	sem_init(&m_Semaphore, 0, Count);
}

GSemaphoreImpl::~GSemaphoreImpl(void)
{
	// Free the semaphore.
	sem_destroy(&m_Semaphore);
}

int GSemaphoreImpl::Wait(long Timeout)
{
	int status;

	// Process wait based on desired timeout behavior.
	switch (Timeout)
	{
		// No wait specified, don't block.
		case 0:
			status = sem_trywait(&m_Semaphore);
			break;

		// Wait forever.
		case -1:
			do
			{
				status = sem_wait(&m_Semaphore);
			} while ((status < 0) && ((EAGAIN == errno) || (EINTR == errno)));
			break;

		// Wait with timeout.
		default:
			struct timespec stTimeout;
			TimeSpec(Timeout, &stTimeout);

			do
			{
				status = sem_timedwait(&m_Semaphore, &stTimeout);
			} while ((status < 0) && ((EAGAIN == errno) || (EINTR == errno)));
			break;
	}

	// Return semaphore acquisition status.
	return (status == 0)? 1 : 0;
}

void GSemaphoreImpl::Post(void)
{
	// Return a count of the semaphore.
	sem_post(&m_Semaphore);
}

void GSemaphoreImpl::TimeSpec(long lmili, struct timespec* stTimeout)
{
	const long NanosPerMilli  = 1000000;
	const long NanosPerSecond = 1000000000;

	// Get the current system time.
	struct timeb now;
	ftime(&now);

	// Set timeout relative to current time.
	stTimeout->tv_sec = now.time + long(lmili / 1000);	/* Seconds */
	stTimeout->tv_nsec = (now.millitm + (lmili % 1000)) * NanosPerMilli;	/* Nanoseconds */

	// Account for nanosecond rollover.
	if (stTimeout->tv_nsec >= NanosPerSecond)
	{
		stTimeout->tv_sec++;
		stTimeout->tv_nsec -= NanosPerSecond;
	}
}
