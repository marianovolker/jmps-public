#include "GMutexUT.h"
#include "GMutex.h"
#include "GSystem.h"
#include "GTask.h"

#include <vector>
using namespace std;

class GMutexTask : public GTask
{
public :
	GMutexTask(GMutexUT& mutexUT):
	   m_mutexUT(mutexUT)
	   {

	   }

	   void Run()
	   {
		   GMutex* pMutex = m_mutexUT.m_pMutex;

		   pMutex->Lock();
		   ++(m_mutexUT.m_ulCounter);
		   pMutex->Unlock();

	   }
private:
	GMutexUT& m_mutexUT;
};

GMutexUT::GMutexUT()
{

}
GMutexUT::~GMutexUT()
{

}

void GMutexUT::SetUp()
{
	m_ulCounter = 0;
	m_pMutex = new GMutex();
}

void GMutexUT::TearDown()
{
	delete m_pMutex;
	m_pMutex = NULL;
}

TEST_F(GMutexUT, TestLockUnlok)
{
	m_pMutex->Lock();
	m_ulCounter = 5;

	const int NumberOfThreads = 10;

	vector<GMutexTask*> tasks;
	for(int i=0; i<NumberOfThreads; ++i)
	{
		GMutexTask* newTask = new GMutexTask(*this);
		newTask->Start(eJoinable);
		tasks.push_back(newTask);
	}

	GSystem::Sleep(2000);
	EXPECT_EQ(5, m_ulCounter);

	m_pMutex->Unlock();

	for( int i=0; i<NumberOfThreads; ++i )
	{
		GMutexTask* task = tasks[i];
		task->Join();
		delete task;
	}

	EXPECT_EQ(15, m_ulCounter);
}

