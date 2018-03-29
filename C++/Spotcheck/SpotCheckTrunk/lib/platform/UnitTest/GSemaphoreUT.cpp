#include <iostream>
#include <sys/types.h>
#include "GSemaphoreUT.h"
#include "GSemaphore.h"
#include "GMutex.h"
#include "GSystem.h"
#include "GTask.h"

#include <errno.h>

using namespace std;

const int GSemaphoreUT::CSemCount = 5;
const int NonRealTimeToleranceMili = 50;

class GSemaphoreTask : public GTask
{
public :
	GSemaphoreTask(GSemaphoreUT& semUT):
	   m_semUT(semUT),
	   m_failAtWait(false)
	{

	}

	void Run()
	{
		GMutex* pMutex = m_semUT.m_pMutex;
		GSemaphore* pSem = m_semUT.m_pSem;
		GMutex* pCounterLock = m_semUT.m_pCounterLock;

		m_failAtWait = false;
		if( !pSem->Wait(m_semUT.m_lTimeout) )
		{
			m_failAtWait = true;
			return;
		}

		pCounterLock->Lock();
		++(m_semUT.m_ulCounter);
		pCounterLock->Unlock();

		//it is just a workaround to force all threads to wait here
		pMutex->Lock();
		pMutex->Unlock();
		pSem->Post();
	}

	bool IsFailAtWait()
	{
		return m_failAtWait;
	}

private:
	GSemaphoreUT& m_semUT;
	bool m_failAtWait;
};

GSemaphoreUT::GSemaphoreUT()
 : m_pMutex(NULL), m_pSem(NULL), m_ulCounter(0), m_lTimeout(0)
{
}

GSemaphoreUT::~GSemaphoreUT()
{

}

void GSemaphoreUT::SetUp()
{
	m_pMutex = new GMutex();
	m_pSem = new GSemaphore(GSemaphoreUT::CSemCount);
	m_pCounterLock = new GMutex();
}

void GSemaphoreUT::TearDown()
{
	delete m_pSem;
	m_pSem = NULL;

	delete m_pMutex;
	m_pMutex = NULL;

	delete m_pCounterLock;
	m_pCounterLock = NULL;
}


TEST_F(GSemaphoreUT, TestInifiniteSemWait)
{
	m_ulCounter = 0;
	m_lTimeout = -1;

	m_pMutex->Lock();

	const int NumberOfThreads = 10;

	vector<GSemaphoreTask*> tasks;
	for(int i=0; i<NumberOfThreads; ++i)
	{
		GSemaphoreTask* newTask = new GSemaphoreTask(*this);
		newTask->Start(eJoinable);
		tasks.push_back(newTask);
	}

	GSystem::Sleep(1500);
	EXPECT_EQ(GSemaphoreUT::CSemCount, m_ulCounter);

	m_pMutex->Unlock();

	int numberOfTasksFailedAtWait = 0;
	for( int i=0; i<NumberOfThreads; ++i )
	{
		GSemaphoreTask* task = tasks[i];
		task->Join();
		if(task->IsFailAtWait()) numberOfTasksFailedAtWait++;
		
		delete task;

	}

	EXPECT_EQ(numberOfTasksFailedAtWait, 0);

	EXPECT_EQ(NumberOfThreads, m_ulCounter);
}

TEST_F(GSemaphoreUT, TestTrySemWait)
{
	m_ulCounter = 0;
	m_lTimeout = 0;

	m_pMutex->Lock();

	const int NumberOfThreads = 10;

	vector<GSemaphoreTask*> tasks;
	for(int i=0; i<NumberOfThreads; ++i)
	{
		GSemaphoreTask* newTask = new GSemaphoreTask(*this);
		newTask->Start(eJoinable);
		tasks.push_back(newTask);
	}


	GSystem::Sleep(200);
	EXPECT_EQ(GSemaphoreUT::CSemCount, m_ulCounter);

	m_pMutex->Unlock();

	for( int i=0; i<NumberOfThreads; ++i )
	{
		GSemaphoreTask* task = tasks[i];
		task->Join();
		delete task;
	}

	EXPECT_EQ(GSemaphoreUT::CSemCount, m_ulCounter);
}

TEST_F(GSemaphoreUT, TestTimeoutSemWait)
{
	m_ulCounter = 0;
	m_lTimeout = 250;

	m_pMutex->Lock();

	const int NumberOfThreads = 10;

	vector<GSemaphoreTask*> tasks;
	for(int i=0; i<NumberOfThreads; ++i)
	{
		GSemaphoreTask* newTask = new GSemaphoreTask(*this);
		newTask->Start(eJoinable);
		tasks.push_back(newTask);
	}


	//We expect that 5 of the timeouts will be hit here
	GSystem::Sleep(m_lTimeout + NonRealTimeToleranceMili);
	EXPECT_EQ(GSemaphoreUT::CSemCount, m_ulCounter);

	m_pMutex->Unlock();

	int numberOfTasksFailedAtWait = 0;
	for( int i=0; i<NumberOfThreads; ++i )
	{
		GSemaphoreTask* task = tasks[i];
		task->Join();
		if(task->IsFailAtWait()) numberOfTasksFailedAtWait++;
		
		delete task;

	}

	EXPECT_EQ(numberOfTasksFailedAtWait, NumberOfThreads-GSemaphoreUT::CSemCount);
	EXPECT_EQ(GSemaphoreUT::CSemCount, m_ulCounter);
}

