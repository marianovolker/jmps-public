// GConditionImpl.h
//
// Windows implementation of GCondition class.


#ifndef GConditionImpl_H
#define GConditionImpl_H


// Include dependent classes.

#include <GMutex.h>
#include <GSemaphore.h>


// GConditionImpl

class GConditionImpl
{
protected:
	// Constructor
	GConditionImpl(GMutex * pMutex);

	// Destructor
	virtual ~GConditionImpl(void);

	// Wait() - Wait for the condition to be signaled.
	int  Wait(long Timeout);

	// Signal() - Signal the condition.
	void Signal(void);

private:
	// Mutex reference for wait/signal exclusion.
	GMutex * m_pMutex;

	// Semaphore used for the signal.
	GSemaphore m_Semaphore;

	// Prohibit GConditionImpl copy.
	GConditionImpl(const GConditionImpl &right);
	GConditionImpl& operator=(const GConditionImpl &right);
};


#endif
