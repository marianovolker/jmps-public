// GTaskImpl.cpp
//
// Windows implementation of the GTask class.

#include <GTaskImpl.h>
#include <GSystem.h>

GTaskId const eInvalidTaskId = 0;

#include <assert.h>
#include <winbase.h>


// GTaskImpl

GTaskImpl::GTaskImpl(eTaskPriority Priority)
		:	m_Id(eInvalidTaskId),
			m_gImplementationId(),
			m_Priority(Priority),
			m_RunMode(eJoinable),
			m_InitCondition(&m_Mutex),
			m_JoinCondition(&m_Mutex),
			m_ExitCondition(&m_Mutex)
{
}

GTaskImpl::~GTaskImpl(void)
{
	m_Id = eInvalidTaskId;
	m_Priority = ePriorityNormal;

	if (m_gImplementationId != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_gImplementationId);
	}
}

void GTaskImpl::Start(eTaskRunMode RunMode)
{
	GMutexLock Lock(m_Mutex);

	// Ensure task hasn't already been started.
	if (m_Id != eInvalidTaskId)
	{
		return;
	}

	// Save task run mode.
	m_RunMode = RunMode;

	m_gImplementationId = CreateThread(0x00, 0x00, GTaskImpl::TaskMain,this, 0x00, 0x00);
	// Fork the task.
	if (m_gImplementationId != INVALID_HANDLE_VALUE)
	{
		// Wait for task initialization.
		while (m_Id == eInvalidTaskId)
		{
			m_InitCondition.Wait();
		}
	}

}

void GTaskImpl::Join(void)
{
	GMutexLock Lock(m_Mutex);

	// Task must be joinable.
	if (m_RunMode != eJoinable)
	{
		return;
	}

	// Should be another task calling.
	if (GSystem::GetTaskId() == m_Id)
	{
		return;
	}

	// Task should be running.
	if (eInvalidTaskId == m_Id)
	{
		return;
	}

	// Signal the join and wait for task exit.
	m_JoinCondition.Signal();
	m_ExitCondition.Wait();
}

void GTaskImpl::Detach(void)
{
	GMutexLock gScopedLock(m_Mutex);

	// Task must currently be joinable.
	if (m_RunMode != eJoinable)
	{
		return;
	}

	// Set task run mode to detached.
	m_RunMode = eDetached;

	// Signal the join, just in case task is waiting.
	m_JoinCondition.Signal();
}

GTaskId GTaskImpl::GetId(void)
{
	GMutexLock gScopedLock(m_Mutex);
	return m_Id;
}

void GTaskImpl::Init(void)
{
	GMutexLock gScopedLock(m_Mutex);

	// Set running task id.
	m_Id = static_cast<GTaskId>(::GetCurrentThreadId());

	// Signal task init.
	m_InitCondition.Signal();
}

eTaskRunMode GTaskImpl::Exit(void)
{
	GMutexLock Lock(m_Mutex);

	// If joinable, wait for the join and signal task exit.
	if (eJoinable == m_RunMode)
	{
		m_JoinCondition.Wait();
		m_ExitCondition.Signal();
	}

	// Return exit run mode.
	return m_RunMode;
}

DWORD GTaskImpl::TaskMain(void *pSelf)
{
	// Get the starting task and arg references.
	GTaskImpl * pTask(static_cast<GTaskImpl*>(pSelf));
	if (pTask)
	{
		// Init and run the task.
		pTask->Init();
		pTask->Run();

		// Perform task exit, deleting any completed detached task.
		if (pTask->Exit() == eDetached)
		{
			delete pTask;
			pTask = NULL;
		}
	}
	return 0;
}
