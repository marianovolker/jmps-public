// GConditionImpl.cpp
//
// Windows implementation of the GCondition class.

#include <GFatalError.h>
#include <GConditionImpl.h>


// GConditionImpl

GConditionImpl::GConditionImpl(GMutex *pMutex)
              : m_pMutex(pMutex),
                m_Semaphore(0)
{
	if (pMutex == NULL)
	{
		GFatalError gFatalError("Mutex NULL pointer for condition.");
	}
}

GConditionImpl::~GConditionImpl(void)
{
}

int GConditionImpl::Wait(long Timeout)
{
	int ret(0);

	// Release the mutex.
	if (m_pMutex)
		m_pMutex->Unlock();

	// Wait for the semaphore.
	ret = m_Semaphore.Wait(Timeout);

	// Re-aquire the mutex.
	if (m_pMutex)
		m_pMutex->Lock();

	return ret;
}

void GConditionImpl::Signal(void)
{
	// Post to the semaphore.
	m_Semaphore.Post();
}
