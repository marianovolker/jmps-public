#include <GMutexImpl.h>
#include <assert.h>

// GMutexImpl
GMutexImpl::GMutexImpl(void)
{
	int iret = pthread_mutex_init(&m_mutex, NULL);
	assert(iret == 0);
}

GMutexImpl::~GMutexImpl(void)
{
	pthread_mutex_destroy(&m_mutex);
}

void GMutexImpl::Lock(void)
{
	int iret = pthread_mutex_lock(&m_mutex);
	assert( iret == 0);
}

void GMutexImpl::Unlock(void)
{
	int iret = pthread_mutex_unlock(&m_mutex);
	assert( iret == 0);
}
