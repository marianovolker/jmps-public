#include <GMutexImpl.h>
#include <assert.h>

// GMutexImpl
GMutexImpl::GMutexImpl(void)
{
	InitializeCriticalSection(&m_mutex);
}

GMutexImpl::~GMutexImpl(void)
{
	 DeleteCriticalSection(&m_mutex);
}

void GMutexImpl::Lock(void)
{
	EnterCriticalSection(&m_mutex);
}

void GMutexImpl::Unlock(void)
{
	LeaveCriticalSection(&m_mutex);
}
