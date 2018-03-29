// GTaskUT.cpp

#include "GTaskUT.h"
#include <GTask.h>
#include <GMutex.h>
#include <GCondition.h>
#include "GSystem.h"

GMutex     TestMutex;
GCondition TestCondition(&TestMutex);

class TestTask : public GTask
{
public:
	TestTask(int Milliseconds)
	   : m_Milliseconds(Milliseconds)
	{
	}

	~TestTask()
	{
		TestMutex.Lock();
		TestCondition.Signal();
		TestMutex.Unlock();
	}

private:
	void Run(void)
	{
		GSystem::Sleep(m_Milliseconds);
	}

	int m_Milliseconds;

	TestTask(const TestTask &);
	TestTask & operator=(const TestTask &);
};


GTaskUT::GTaskUT()
{
}

GTaskUT::~GTaskUT()
{
}

void GTaskUT::SetUp()
{
}

void GTaskUT::TearDown()
{
}

// TestJoinable - Test function and exit of joinable task.

TEST_F(GTaskUT, TestJoinable)
{
	GMutexLock Lock(TestMutex);
	TestTask * pTask(new TestTask(3000));
	int iRet(0);

	pTask->Start(eJoinable);

	GTaskId TaskId = pTask->GetId();
	ASSERT_NE(TaskId,eInvalidTaskId);

	// Don't expect signal (before join)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet, 0);

	pTask->Join();

	// Don't expect signal (after join)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet,0);

	TestMutex.Unlock();
	delete pTask;
	TestMutex.Lock();

	// Expect signal (after delete)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet,1);
}

// TestDetached - Test function and exit of detached task.

TEST_F(GTaskUT, TestDetached)
{
	GMutexLock Lock(TestMutex);
	TestTask* pTask = new TestTask(3000);
	int iRet(0);

	pTask->Start(eDetached);

	GTaskId TaskId = pTask->GetId();
	ASSERT_NE(TaskId,eInvalidTaskId);

	// Expect signal (without join)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet, 1);
}


// TestPreDetach - Test function and exit of joinable task that is detached before task exit.

TEST_F(GTaskUT, TestPreDetach)
{
	GMutexLock Lock(TestMutex);
	TestTask * pTask(new TestTask(3000));
	int iRet(0);

	pTask->Start(eJoinable);

	GTaskId TaskId = pTask->GetId();
	ASSERT_NE(TaskId,eInvalidTaskId);

	pTask->Detach();

	// Expect signal (after detach)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet,1);
}


// TestPostDetach - Test function and exit of joinable task that is detached after task exit.

TEST_F(GTaskUT, TestPostDetach)
{
	GMutexLock Lock(TestMutex);
	TestTask * pTask(new TestTask(3000));
	int iRet(0);

	pTask->Start(eJoinable);

	GTaskId TaskId = pTask->GetId();
	ASSERT_NE(TaskId,eInvalidTaskId);

	// Don't expect signal (before detach)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet, 0);

	pTask->Detach();

	// Expect signal (after detach)
	iRet = TestCondition.Wait(5000);
	EXPECT_EQ(iRet,1);
}
