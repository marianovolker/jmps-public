// GMutexImpl.h
//
// Windows implementation of the mutex interface using mingw

#ifndef GMutexImpl_h
#define GMutexImpl_h

#include <GWindows.h>

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
	CRITICAL_SECTION m_mutex;

	// Prohibit GMutex copy.
	GMutexImpl(const GMutexImpl &right);
	GMutexImpl& operator=(const GMutexImpl &right);
};

#endif /* GMutexImpl_h */
