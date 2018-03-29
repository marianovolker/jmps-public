// GTaskImpl.cpp
//
// Windows implementation of the GTask class.

#include <GTaskImpl.h>
#include <GSystem.h>

GTaskId const eInvalidTaskId = 0;

#include <assert.h>
#include <sched.h>


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
}

void GTaskImpl::Start(eTaskRunMode RunMode)
{
	GMutexLock Lock(m_Mutex);

	// Ensure task hasn't already been started.
	if (m_Id != eInvalidTaskId)
	{
		return;
	}

	pthread_attr_t tattr;
	pthread_attr_t * ptattr(NULL);

	if (pthread_attr_init(&tattr) == 0)
	{
		sched_param tparam;
		ptattr = &tattr;
		if (pthread_attr_getschedparam(ptattr,&tparam) == 0)
		{
			// Get thread scheduling policy.
			int iPolicy(-1);
			if (pthread_attr_getschedpolicy(ptattr,&iPolicy) == 0)
			{
				// Get nominal priority values based on scheduling policy.
				int iSchedPrioMax(sched_get_priority_max(iPolicy));
				int iSchedPrioMin(sched_get_priority_min(iPolicy));
				int iSchedPrioMed(static_cast<int>(iSchedPrioMax - iSchedPrioMin)/2);
				switch (m_Priority)
				{
					case ePriorityLow:
						tparam.sched_priority = iSchedPrioMin;
						break;

					case ePriorityHigh:
						tparam.sched_priority = iSchedPrioMax;
						break;

					case ePriorityNormal:
					default:
						tparam.sched_priority = iSchedPrioMed;
						break;
				}
			}
			pthread_attr_setschedparam (ptattr, &tparam);
		}

		// Always configure pthread as detached. We will handle joinable
		// support in this class, independent of pthread's implementation.
		pthread_attr_setdetachstate(ptattr, PTHREAD_CREATE_DETACHED);

		// Save task run mode.
		m_RunMode = RunMode;

		// Fork the task.
		if (pthread_create(&m_gImplementationId,ptattr,GTaskImpl::TaskMain,this) == 0)
		{
			// Wait for task initialization.
			while (m_Id == eInvalidTaskId)
			{
				m_InitCondition.Wait();
			}
		}

		if (ptattr != NULL)
		{
			pthread_attr_destroy(ptattr);
			ptattr = NULL;
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
	m_Id = static_cast<GTaskId>(pthread_self());

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

void * GTaskImpl::TaskMain(void *pSelf)
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
	return NULL;
}
