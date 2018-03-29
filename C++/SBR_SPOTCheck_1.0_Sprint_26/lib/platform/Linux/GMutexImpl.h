// GMutexImpl.h
//
// Linux implementation of the GMutex class.

#ifndef GMutexImpl_h
#define GMutexImpl_h

#include <GLinux.h>
#include <pthread.h>

// GMutexImpl

class GMutexImpl
{
protected:
	// Constructor
	GMutexImpl(void);

	// Destructor
	virtual ~GMutexImpl(void);

	// Lock() - Lock the mutex.
	void Lock(void);

	// Unlock() - Unlock the mutex.
	void Unlock(void);

private:
	pthread_mutex_t m_mutex;

	// Prohibit GMutex copy.
	GMutexImpl(const GMutexImpl &right);
	GMutexImpl& operator=(const GMutexImpl &right);
};

#endif /* GMutexImpl_h */
