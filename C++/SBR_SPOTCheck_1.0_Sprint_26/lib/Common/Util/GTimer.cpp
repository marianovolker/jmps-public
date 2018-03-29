//=============================================================================
// 
// File: GTimer.cpp
// 
// Description: Contains the implementation of the GTimer class.
// 
// Created: 05/20/98
// 
// Author: Jon Towne
//		   Major redesign of implementation by Danny McCorquodale - 3/02
// 
//=============================================================================

#include "Common/Util/GTimer.h"
#include "Common/Util/DEBUGOUT.h"

#include <stdlib.h>

GMutex GTimer::mst_InitMutex;

GTimer::GTimer ()
      : m_ConditionVar(&m_Mutex),
        m_bTimerRunning(false),
        m_bOneShot(false),
        m_pCommand(NULL),
        m_u32MillisecondInterval(0),
        m_bShuttingdown(false),
        m_bThreadStarted(false),
        m_bStopInProgress(false),
        m_pInitConditionVar(NULL)
{
	// This will not return until the timer's thread has entered run_undetached().
	CommonConstructor();
}

GTimer::GTimer (const GAbsCommand* pCommand, UINT32 u32MillisecondInterval, bool bOneShot, bool bAutoStart)
      : m_ConditionVar(&m_Mutex),
        m_bTimerRunning(false),
        m_bOneShot(false),
        m_pCommand(NULL),
        m_u32MillisecondInterval(0),
        m_bShuttingdown(false),
        m_bThreadStarted(false),
        m_bStopInProgress(false),
        m_pInitConditionVar(NULL)
{
    m_pCommand					= const_cast<GAbsCommand*>(pCommand); 
    m_u32MillisecondInterval	= u32MillisecondInterval; 
    m_bOneShot					= bOneShot; 

	// This will not return until the timer's thread has entered run_undetached().
    CommonConstructor();
    
    if(bAutoStart)
    {
        Start();
    }
}

GTimer::~GTimer ()
{
	m_Mutex.Lock();

	// Set flags to indicate shutdown and signal condition var to wake timer's
	// thread up.
	m_bShuttingdown = true;
	m_bTimerRunning = false;
	m_ConditionVar.Signal();

	m_Mutex.Unlock();

	// Wait for task exit.
	Join();
}

void GTimer::CommonConstructor ()
{
	// The timer's thread locks the mutex prior to setting the flag thread started
	// flag and signaling the condition.  So locking it here prevents a scenario
	// where the thread calling this method is just about to enter the wait and a
	// context switch allows the timer's thread to signal the condition first. The
	// mutex_lock object locks the mutex at construction and unlocks it at
	// destruction.  The wait operation unlocks the mutex on entry and locks it on exit.
	mst_InitMutex.Lock();

	m_pInitConditionVar = new GCondition(&mst_InitMutex);
	if (m_pInitConditionVar != NULL)
	{
		// Start execution of the timer's thread.
		GTask::Start(eJoinable);

		// Wait on the condition variable until the timer's thread has begun
		// execution.  Th timer's thread run() method sets the flag and signals the
		// condition variable.  The flag is *ESSENTIAL* to avoid a potential race
		// condition. After the timer's thread is started above, it is up to the scheduler
		// to decide what comes first:  the timer's thread setting the flag and signaling
		// the condition, or this method's calling thread executing the code below.
		// If the wait was unconditional, the thread calling this method would block
		// indefinitely in the case where the timer's thread signaled the condition
		// first.
		while(m_bThreadStarted == false)
		{
			m_pInitConditionVar->Wait();
		}
		delete m_pInitConditionVar;
		m_pInitConditionVar = NULL;
	}
	mst_InitMutex.Unlock();
}

void GTimer::SetCommand (const GAbsCommand* pCommand)
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	m_pCommand = const_cast<GAbsCommand*>(pCommand);
}

GAbsCommand* GTimer::GetCommand ()
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	return(m_pCommand);
}

void GTimer::SetInterval (UINT32 u32Milliseconds)
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	m_u32MillisecondInterval = u32Milliseconds;
}

UINT32 GTimer::GetInterval ()
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	return(m_u32MillisecondInterval);
}

void GTimer::SetOneShot (bool bOneShot)
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	m_bOneShot = bOneShot;
}

bool GTimer::GetOneShot ()
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	return(m_bOneShot);
}

bool GTimer::Start ()
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	bool bReturnVal = false;

	if (m_pCommand == NULL)
	{
		// Would it be appropriate to abort() here?
		const char* szMethodName = "GTimer::Start()";
		LOG(eTimer, eError, "\n\n%s:  command pointer is NULL!!!  *NOT* starting timer!!\n\n", szMethodName);
	}
	else if (!m_bTimerRunning)
	{
		// The timer isn't already running.
		m_bTimerRunning = true;

		// This wakes up the timer's thread.
		m_ConditionVar.Signal();
		
		bReturnVal = true;
	}

	return(bReturnVal);
}

bool GTimer::Stop ()
{
	m_bStopInProgress = true;

	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	bool bReturnVal = false;

    if (m_bTimerRunning)
	{
		// The timer isn't already stopped.
		m_bTimerRunning = false;

		// This wakes up the timer's thread.
		m_ConditionVar.Signal();
		
		bReturnVal = true;
	}

    m_bStopInProgress = false;

	return(bReturnVal);
}

bool GTimer::IsRunning ()
{
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	return(m_bTimerRunning);
}

void GTimer::Reset ()
{
	// Stop the timer from it's current countdown and restart it.  These methods
	// lock m_Mutex before executing.
    Stop();
    Start();
}

void GTimer::Block ()
{
	if (m_u32MillisecondInterval != 0)
	{
		m_Mutex.Lock();
		long ulIntervalMsec = (long)m_u32MillisecondInterval;
		m_Mutex.Unlock();

		GSystem::Sleep(ulIntervalMsec);
	}
}

void GTimer::RemoveEvent ()
{
	// Stop the timer in case it's running.
	Stop ();

	// Because there is a "gap" between the Stop() operation and locking the 
	// mutex below, this operation is NOT atomic.  So it would be possible for the
	// timer to be restarted by another thread prior to setting the command to 
	// NULL.  A solution for this would be duplicating the Stop() logic here or 
	// providing an "internal" stop helper method that could be called by Stop() 
	// and this method.
	GConditionalMutexLock Lock(m_Mutex, ! IsCallerTimersThread());

	m_pCommand = NULL;
}

void GTimer::Run()
{
	mst_InitMutex.Lock();

	m_Mutex.Lock();
    m_bThreadStarted = true;

	// Let the thread driving CommonConstructor() know that the timer's thread
	// has begun execution.
    if (m_pInitConditionVar != NULL)
    {
    	m_pInitConditionVar->Signal();
    }
	mst_InitMutex.Unlock();

	unsigned long	ulIntervalMsec(0);	    // Seconds component of interval
	bool			bTimedOut(false);	// Did the timer expire or was it signaled?

    while(true)
    {
		while( m_bStopInProgress || ! (m_bTimerRunning || m_bShuttingdown) )
		{
			// As long as the timer is not running or being destroyed, block in a
			// "stopped" state.  Note that wait unlocks m_Mutex on entry and locks
			// it on return.  This allows public methods such as Start() to lock 
			// the mutex and execute.
			m_ConditionVar.Wait();
		}
        
        if(m_bShuttingdown)
        {
			// Allow the task to exit.
            break;
        }

		if (m_u32MillisecondInterval != 0)
		{
			// Note that the m_Mutex is unlocked on entry to wait() and locked 
			// on return from wait().  This allows public methods such as Stop() 
			// to lock the mutex and execute.
			bTimedOut = (! m_ConditionVar.Wait(m_u32MillisecondInterval));
		}
		else
		{
			// If the interval is 0, the timer will fire immediately.
			bTimedOut = true;
		}

		if (m_bTimerRunning && bTimedOut)
		{
			if (m_bOneShot)
			{
				// A "one shot" timer disables itself after firing once.  It is 
				// important to do this PRIOR to executing the command because the
				// command itself may restart the timer.
				m_bTimerRunning = false;
			}

			m_pCommand->Execute();
		}
    }

	m_Mutex.Unlock();
}
