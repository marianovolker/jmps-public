// GFatalErrorImpl.cpp

#include <GFatalErrorImpl.h>
#include <GSemaphore.h>
#include <GSystem.h>


GFatalErrorImpl::GFatalErrorImpl(char const * ckpMessage)
{
	// Log fatal error message.
	GSystem::Trace(ckpMessage);

	// Block forever.
	GSemaphore Sem(0);
	Sem.Wait();
}

GFatalErrorImpl::~GFatalErrorImpl(void)
{
}
