// GCondition.h

#ifndef GCondition_h
#define GCondition_h


#include <GConditionImpl.h>


// GCondition
//
// Standard application condition interface.

class GCondition : public GConditionImpl
{
public:
	// Constructor
	GCondition(GMutex *pMutex) : GConditionImpl(pMutex)
	{
	}

	// Destructor
	~GCondition(void)
	{
	}

	// Wait() - Wait for the condition to be signaled. If a mutex
	//          reference is provided on construction, the mutex is
	//          unlocked before the wait and re-locked after wait
	//          completion.
	// params: Timeout = Time to wait in milliseconds, default=forever.
	// return: 1 -> Condition was signaled, 0 -> Condition not signaled.
	int Wait(long Timeout = -1)
	{
		return GConditionImpl::Wait(Timeout);
	}

	// Signal() - Signal the condition.
	void Signal(void)
	{
		GConditionImpl::Signal();
	}

private:
	// Prohibit GCondition copy.
	GCondition(const GCondition &right);
	GCondition& operator=(const GCondition &right);
};


#endif
