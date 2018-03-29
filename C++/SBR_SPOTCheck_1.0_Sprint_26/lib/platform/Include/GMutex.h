// GMutex.h

#ifndef GMutex_h
#define GMutex_h

#include "GMutexImpl.h"


// GMutex
//
// Standard application mutex interface. After construction,
// the mutex should be unlocked and ready for use.

class GMutex : private GMutexImpl
{
public:
	// Constructor
	GMutex(void) : GMutexImpl()
	{
	}

	// Destructor
	~GMutex(void)
	{
	}

	// Lock() - Lock the mutex.
	void Lock(void)
	{
		GMutexImpl::Lock();
	}

	// Unlock() - Unlock the mutex.
	void Unlock(void)
	{
		GMutexImpl::Unlock();
	}

private:
	// Prohibit GMutex copy.
	GMutex(const GMutex &right);
	GMutex& operator=(const GMutex &right);
};


// GMutexLock
//
// The mutex lock class implements a scope-based mutex lock
// for the mutex reference provided. The mutex references
// remains locked as long as the GMutexLock is in scope.

class GMutexLock
{
public:
	// Constructor
	GMutexLock(GMutex &Mutex) : m_Mutex(Mutex)
	{
		// Lock the mutex.
		m_Mutex.Lock();
	}

	// Destructor
	~GMutexLock(void)
	{
		// Unlock the mutex.
		m_Mutex.Unlock();
	}

private:
	// Reference to mutex.
	GMutex& m_Mutex;

	// Prohibit GMutexLock copy.
	GMutexLock(const GMutexLock &right);
	GMutexLock& operator=(const GMutexLock &right);
};


#endif
